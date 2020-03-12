#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <assert.h>


using namespace std;

class Graph {
public:
    Graph() = default;

    Graph(int N){
        resize(N);
    }

    int MakeYourGraphSimpleAgain(){
        int counter = 0;

        return collector(graph, regraph);
    }

    //добавляем ребра в граф
    void add(int from, int to){

        if(from == to) return;

        if (max(from, to) >= graph.size())
            resize(max(from, to));

        graph[from].insert(to);
        regraph[to].insert(from);
    }

private:
    // глобальные переменные
    int time = 0;


    //глобальные контейнеры
    vector<set<int>> graph;
    vector<set<int>> regraph;
    vector<int> colors;                 // 0 - белый 1 - серый ( выполняется ) 2 - черный ( был пройден )
                                        // 3 - бурмалиновый ( был присвоен компоненте сильной связанности )
    queue<int> FinishStack;
    vector<int> TimeIn;
    vector<int> TimeOut;

    //special add       I`m SO  special !!!
    void add(int from, int to, vector<set<int>>& components, vector<set<int>>& recomponents){

        if(from == to) return;

        if (max(from, to) >= components.size()){
            resize(max(from, to) + 1, components, recomponents);
        }

        components[from].insert(to);
        recomponents[to].insert(from);
    }

    void resize(int N){
        graph.resize(N);
        regraph.resize(N);
        colors.resize(N);
        TimeIn.resize(N);
        TimeOut.resize(N);
    }

    //special resize so special ! (no)
    void resize(int N, vector<set<int>>& components, vector<set<int>>& recomponents){
        components.resize(N);
        recomponents.resize(N);
        colors.resize(N);
        TimeIn.resize(N);
        TimeOut.resize(N);
    }

    void prepare(){
        time = 0;

        while (!FinishStack.empty())
            FinishStack.pop();

        colors.assign(colors.size(), 0);
    }

    static void clearComponents(vector<set<int>>& components, vector<set<int>>& recomponents){
        components.erase(components.begin(), components.end());
        recomponents.erase(recomponents.begin(), recomponents.end());
    }

    //обход без захвата ребер
    void DFS (int vertex, const vector<set<int>>& components){
        if (colors[vertex] != 0) return;
        colors[vertex] = 1;
        TimeIn[vertex] = ++time;

        for (auto friends : components[vertex]){
            DFS(friends, components);
        }

        FinishStack.push(vertex);
        colors[vertex] = 2;
        TimeOut[vertex] = ++time;
    }

    //обход вместе с захватом ребер ведущих в другую компоненту
    void DFS (int vertex, const vector<set<int>>& components, vector<set<int>>& box){
        if (colors[vertex] == 3) box[box.size() - 1].insert(vertex);
        if (colors[vertex] != 0) return;
        colors[vertex] = 1;
        TimeIn[vertex] = ++time;

        for (auto friends : components[vertex]){
            DFS(friends, components, box);
        }

        FinishStack.push(vertex);
        colors[vertex] = 2;
        TimeOut[vertex] = ++time;
    }

    int collector (vector<set<int>>& components, vector<set<int>>& recomponents){
        vector<set<int>> box;
        vector<set<int>> clusters;

        TimeWriter(components);

        clusterBuilder(clusters, components, recomponents, box);

        condensation(components, recomponents, clusters, box);

        if (clusters.size() == 1) return  0;

        return max(rootSicker(recomponents), diver(components));
    }

    // Собирает все всех: время записывальщика, строителя кластеров, конденсатора, строителя мостов, да, почти все контейнеры гуляют ссылками
    void collector(vector<set<int>>& components, vector<set<int>>& recomponents, int& counter){
        vector<set<int>> box;
        vector<set<int>> clusters;

        TimeWriter(components);

        clusterBuilder(clusters, components, recomponents, box);

        condensation(components, recomponents, clusters, box);

        if (components.empty() and clusters.size() == 1)
            return;

        BridgeBuilder(components, recomponents);

        ++counter;

        collector(components, recomponents, counter);
    }

    // строит кластеры ( компоненты сильной связанности )
    void clusterBuilder(vector<set<int>>& clusters, vector<set<int>>& components, vector<set<int>>& recomponents, vector<set<int>>& box){

        int istart = 0;
        for (int i = 0; i < TimeOut.size(); ++i) {
            if (TimeOut[i] > TimeOut[istart])
                istart = i;
        }

        if (TimeOut[istart] == 0)
            return;

        box.resize(box.size() + 1);

        assert(FinishStack.empty());

        DFS(istart, recomponents, box);

        clusters.emplace_back();
        while (!FinishStack.empty()){
            int clusterItem = FinishStack.front();
            FinishStack.pop();
            clusters[clusters.size() - 1].insert(clusterItem);

            colors[clusterItem] = 3;
            TimeOut[clusterItem] = 0;
        }

        clusterBuilder(clusters, components, recomponents, box);
    }

    // конденсирует граф
    void condensation(vector<set<int>>& components, vector<set<int>>& recomponents, vector<set<int>>& clusters, vector<set<int>>& box){
        vector<set<int>> temp(clusters.size());
        vector<set<int>> retemp(clusters.size());

        for (int to = 0; to < clusters.size(); ++to){
            for(auto node : box[to])
                add(FindHimCluster(node, clusters), to, temp, retemp);
        }



        components = temp;
        recomponents = retemp;
    }

    //найдет кластер в котором лежит нода son
    static int FindHimCluster(int& son, vector<set<int>>& clusters){
        for (int i = 0; i < clusters.size(); ++i) {
            if (clusters[i].count(son) > 0)
                return i;
        }
        return -1;
    }

    //первый обход графа для записи времени выхода
    void TimeWriter(vector<set<int>>& components){
        prepare();
        for (int i = 0; i < components.size(); i++)
            DFS(i, components);
        prepare();
    }

    //строит цикл в ацкиличном графе, оптимально добавляя ребро
    void BridgeBuilder(vector<set<int>>& components, vector<set<int>>& recomponents){
        int endPoint = rootSicker(recomponents);
        int startPoint = diver(components);
        add(startPoint, endPoint, components, recomponents);
    }

    //ищет ноду в подвешенном(ацикличном) графе в которую никто не приходит
    //минимальный в топологической сортировке
    static int rootSicker(const vector<set<int>>& recomponents){
        int count = 0;
        for (const auto & recomponent : recomponents){
            if (recomponent.empty()) ++count;
        }
        return count;
    }

    //ищет ноду в подвешенном(ацикличном) графе из которой никто не выходит
    //максимальный в тополгической сортировке
    static int diver(const vector<set<int>>& components){
        int count = 0;
        for (const auto & component : components){
            if (component.empty()) ++count;
        }
        return count;
    }

};

int main() {
    int M, N, from, to;

    cin >> N >> M;

    Graph Streets(N);

    for (int i = 0; i < M; ++i) {
        cin >> from >> to;
        Streets.add (from - 1, to - 1);
    }

    cout << Streets.MakeYourGraphSimpleAgain();
    return 0;
}
