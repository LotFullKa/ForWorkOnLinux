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

    // добавляет ребра(вместе с ними и точки) в граф
    void add(int from, int to){
        if (max(from, to) + 1 > graph.size())
            resize(max(from, to) + 1);
        graph[from].insert(to);
        regraph[to].insert(from);
    }

    int MakeYourGraphSimpleAgain (){
        int counter = 0;
        collectorGraph(graph, regraph, counter);
        return counter;
    }


    // выводит кластеры ( удобно )
    void classtorPrint(){
        vector<set<int>> Classters;
        classtorBuildeer(regraph, Classters);
        for(int i = 0; i < Classters.size(); ++i){
            cout << " " << i <<" : { ";
            for (int g : Classters[i]) {
                cout << g << " ";
            }
            cout << "}" << endl;
        }
        prepare();
    }

    // не пугайся, это для вызова с аргументом функции выше
    void classtorPrint(vector<set<int>> Classters){
        for(int i = 0; i < Classters.size(); ++i){
            cout << " " << i <<" : { ";
            for (int g : Classters[i]) {
                cout << g << " ";
            }
            cout << "}" << endl;
        }
    }

    // выводит столбец графов
    void print(){

        int max = 0;
        for (const set<int>& i : regraph)
            if (max < i.size()) max = i.size();

        int k = 0;
        cout << endl;
        for (const set <int>& i : graph) {
            for (int l = 0; l < (max - regraph[k].size()); ++l) cout << "  ";
            for (int g : regraph[k]) cout << g << " ";
            cout << ":" << k++ << ": ";
            for (int g : i) cout << g << "  ";
            cout << endl;
        }
    }

    /* выводит последоватьельность зависящих графов
     * в случае несвязанного графа выведет вместе, разбирайся сам
     * заполняет TimeIn и TimeOut
     */
    void finishprint(){
        prepare();
        for(int i = 0; i < graph.size(); i++)
            DFS(i, graph);
        for (int i = 0; i < graph.size(); ++i) {
            cout << FinishStack.front() << "  ";
            FinishStack.pop();
        }
        cout << endl;

        cout << "TimeIn : " << "[ ";
        for (int i : TimeIn){
            cout << i << " ";
        }
        cout << "]" << endl;

        cout << "TimeOut : " << "[ ";
        for (int i : TimeOut){
            cout << i << " ";
        }
        cout << "]" << endl;
        prepare();
    }
private:

    int time = 1;


    vector<set <int>>  graph;              // под цифрой лежат цифры куда можной пойти
    vector<set <int>>  regraph;
    vector<int> colors;                     // 0 - белый   1 - серый    2 - черный  3 - бурмалиновый
    vector<int> TimeIn;
    vector<int> TimeOut;
    queue <int>  FinishStack;
    vector<set<int>> tempEdges;
    vector<set<int>> clusterEdges;
    vector<set<int>> reclusterEdges;

    void resize(int N){
        regraph.resize(N);
        graph.resize(N);
        colors.resize(N, 0);
        TimeIn.resize(N);
        TimeOut.resize(N);
        tempEdges.resize(N);
        clusterEdges.resize(N);
        reclusterEdges.resize(N);
    }

    void prepare(){
        while (!FinishStack.empty()){
            FinishStack.pop();
        }
        colors.assign(colors.size(), 0);
        time = 0;
        tempEdges.assign(tempEdges.size(), set<int>());
        reclusterEdges.assign(reclusterEdges.size(), set<int> ());
        clusterEdges.assign(clusterEdges.size(), set<int>());
    }

    int DFS(int s, vector<set<int>> & graphToRun) {
        if (colors[s] == 3) return s;
        if(colors[s] != 0) return -1;
        colors[s] = 1;
        TimeIn[s] = time;
        time ++;

        for (auto i : graphToRun[s]) {
            int x = DFS(i, graphToRun);
            if (x != -1) tempEdges[s].insert(x);
        }

        TimeOut[s] = time;
        time++;
        colors[s] = 2;
        FinishStack.push(s);
        return -1;
    }

    // находит корень в подвешенном граффе
    static int  rootSicker(const vector<set<int>> &recomponents){
        for (int i = 0; i < recomponents.size(); ++i) {
            if (recomponents[i].empty())
                return i;
        }
        return -1;
    }

    // в подвешенном графе находит самую дальную ноду от startPoint (рекурсивная)
    static int diver(const vector<set<int>>& components, int startPoint, vector<int>& deep){

        for (int i : components[startPoint]){
                deep[i] += 1;
                diver(components, i, deep);
            }

        int bottom = 0;
        for (int i = 0; i < deep.size(); ++i) {
            if (deep[i] > deep[bottom]){
                bottom = i;
            }
        }
        return bottom;
    }

    // строит ребро, которое создает цикл максимальной длинны (по сути еще одну компоненту сильной связанности)
    void bridgeBuilder(vector<set<int>>& components, vector<set<int>> &recomponents){
        vector<int> deep(components.size());
        int startPoint = rootSicker(recomponents);
        int endPoint = diver(components, startPoint, deep);
        components[endPoint].insert(startPoint);
        recomponents[startPoint].insert(endPoint);
    }



    //рекурсивно строит клаассторы ! С условие что по графу прошлись DFS и заполнили TimeOut
    int classtorBuildeer(vector<set<int>> &reverseGraph, vector<set<int>>& Classters){

        int istarer = 0;
        int max = TimeOut[0];

        for (int i = 0; i < TimeOut.size(); ++i) {
            if (max < TimeOut[i]){
                max = TimeOut[i];
                istarer = i;
            }
        }

        if(max == 0) return Classters.size();

        assert(FinishStack.empty());

        DFS(istarer, reverseGraph);

        Classters.resize(Classters.size() + 1);
        while(!FinishStack.empty()) {
            int clusteritem = FinishStack.front();
            Classters[Classters.size() - 1].insert(clusteritem);
            FinishStack.pop();
            if (!tempEdges[clusteritem].empty()){
                for (int friends : tempEdges[clusteritem]) {
                    int x = FindHimCluster(friends, Classters);
                    if (x != -1) {
                        clusterEdges[Classters.size() - 1].insert(x);
                        reclusterEdges[x].insert(Classters.size() - 1);
                    }
                }
            }
            colors[clusteritem] = 3;
            TimeOut[clusteritem] = 0;
        }

        classtorBuildeer(reverseGraph, Classters);
        return Classters.size();
    }

    //вернет индекс класетра с котором связан, спрашивающий
    int FindHimCluster(int vertex, const vector<set<int>>& cluster){
        for (int i = 0; i < cluster.size(); ++i) {
            if (cluster[i].count(vertex) != 0) return i;
        }
        return -1;
    }

    // @@@@@@@@@@ Должен собирать из графа и реграфа
    void collectorGraph (vector<set<int>>& components, vector<set<int>>& recomponents, int &counter) {
        vector<set<int>> Cluster;

        prepare();
        for (int i = 0; i < components.size(); i++)
            DFS(i, components);
        prepare();

        classtorBuildeer(recomponents, Cluster);

        components = clusterEdges;
        recomponents = reclusterEdges;

        if (simpleChecker(components)) return;

        bridgeBuilder(components, recomponents);

        ++counter;

        collectorGraph(components, recomponents, counter);
    }

    bool simpleChecker(vector<set<int>>& components) {
        for (const auto& item : components)
            if (!item.empty()) return false;
        return true;
    }
};

int main() {
    int m, n, in, out;
    cin >> n >> m;

    Graph Cities(n);

    for (int i = 0; i < m; ++i){
        cin >> in  >> out;
        Cities.add(in - 1 , out - 1);
    }

    cout << Cities.MakeYourGraphSimpleAgain();

    return 0;
}
