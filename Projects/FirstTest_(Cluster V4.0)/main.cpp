#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <assert.h>

using namespace std;

class Graph{
public:

    Graph() = default;

    Graph(int N){
        resize(N);
    }

    //добавляем ребра в граф
    void add(int from, int to){

        if(from == to) return;

        if (max(from, to) >= graph.size())
            resize(max(from, to));

        graph[from].insert(to);
        regraph[to].insert(from);
    }

    int MakeYourGraphSimpleAgain(){
        return collector(graph, regraph);
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
    vector<int> TimeOut;


    void resize(int N){
        graph.resize(N);
        regraph.resize(N);
        colors.resize(N);
        TimeOut.resize(N);
    }

    //special add       I`m SO  special !!!
    static void add(int from, int to, vector<set<int>>& components, vector<set<int>>& recomponents){

        if(from == to) return;

        components[from].insert(to);
        recomponents[to].insert(from);
    }

    //обход без захвата ребер
    void DFS (int vertex, const vector<set<int>>& components){
        if (colors[vertex] != 0) return;
        colors[vertex] = 1;
        ++time;

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
        ++time;

        for (auto friends : components[vertex]){
            DFS(friends, components, box);
        }

        FinishStack.push(vertex);
        colors[vertex] = 2;
        TimeOut[vertex] = ++time;
    }

    void prepare(){
        time = 0;

        while (!FinishStack.empty())
            FinishStack.pop();

        colors.assign(colors.size(), 0);
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

    //первый обход графа для записи времени выхода
    void TimeWriter(vector<set<int>>& components){
        prepare();
        for (int i = 0; i < components.size(); i++)
            DFS(i, components);
        prepare();
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
    static void condensation(vector<set<int>>& components, vector<set<int>>& recomponents, vector<set<int>>& clusters, vector<set<int>>& box){

        for (int to = 0; to < clusters.size(); ++to){
            for(auto node : box[to])
                add(FindHimCluster(node, clusters), to, components, recomponents);
        }
    }

    //найдет кластер в котором лежит нода son
    static int FindHimCluster(int& son, vector<set<int>>& clusters){
        for (int i = 0; i < clusters.size(); ++i) {
            if (clusters[i].count(son) > 0)
                return i;
        }
        return -1;
    }

    //вернет количество начальных ущлов
    static int rootSicker(const vector<set<int>>& recomponents){
        int count = 0;
        for (const auto & recomponent : recomponents){
            if (recomponent.empty()) ++count;
        }
        return count;
    }

    //вернет количество тупиковых узлов
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
}
