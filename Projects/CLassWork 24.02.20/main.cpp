/*
 * Сегодня только неориентированные графы
 *
 * определения:
 *
 * Точка сочлинения - при удалении этой ноды количество компанент связанности увеличивается
 * мост - при удалении этого ребра количество компонент связанности увеличивается
 * ребра е1 и е2 находятся в отношении R, на графе G, если найлется цикл, который их содержит
 */

#include <iostream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

class Graph {
public:

    explicit Graph(int N = 1){
        resize(N);
    }

    void add(int from, int to){
        if (max(from, to) + 1 > graph.size())
            resize(max(from, to) + 1);
        graph[from].insert(to);
        graph[to].insert(from);
    }

    void print(){

        int k = 0;
        cout << endl;
        for (const set <int>& i : graph) {
            cout << " " << k++ << ": ";
            for (int g : i) cout << g << "  ";
            cout << endl;
        }


    }

    void Junction_points_print(){
        prepare();
        DFS_Junction(0, -1);
        cout << "Точки сочлинения: ";
        for (int i : junction_points)
            cout << i << " ";
        cout << endl;
    }

    void Bridges_print(){
        prepare();
        DFS_Brifge(0);
        cout << "Мосты :" << endl;
        for (const vector<int>& i : bridges) {
            for (int g : i)
                cout << g << " ";
            cout << endl;
        }
    }

private:

    int time = 0;
    vector<set<int>>  graph;

    vector<int> TimeIn;
    vector<int> colors;
    vector<int> low;
    stack<int> finish_stack;
    set<int> junction_points;
    set<vector<int>> bridges;


    void resize(int N){
        graph.resize(N);
        colors.assign(N,0);
        TimeIn.resize(N);
        low.resize(N);
    }

    void prepare(){
        colors.assign(graph.size(), 0);
        time = 0;
    }


    void DFS_Junction(int me, int parent) {
        colors[me] = 1;
        time++;
        TimeIn[me] = time;
        low[me] = time;
        int children = 0;
        for (int v : graph[me]){
            if (v != parent){
                if (colors[v] == 0){
                    DFS_Junction(v, me);
                    children++;
                    low[me] = min(low[me], low[v]);
                    if (parent >= 0 and low[v] >= TimeIn[me])
                        junction_points.insert(me);
                } else {
                    low[me] = min(low[me], TimeIn[v]);
                }
            }
        }

        if (parent < 0 and children >= 2)
            junction_points.insert(me);
        colors[me] = 2;
    }

    void DFS_Brifge(int me){
        colors[me] = 1;
        TimeIn[me] = ++time;
        low[me] = time;
        for (int ver : graph[me]){
            if (colors[me] == 0){
                DFS_Brifge(ver);
                low[me] = min(low[me], low[ver]);
                if (low[ver] > TimeIn[me] and low[ver] >= TimeIn[ver]) {
                    vector<int> pair;
                    pair.push_back(me);
                    pair.push_back(ver);
                    bridges.insert(pair);
                }
            } else {
                low[me] = min(low[me], TimeIn[ver]);
            }
        }
        colors[me] = 2;
    }
};

int main() {
    Graph Nodes;

    int in, out;
    while(cin >> in >> out){
        if (in == -1) break;
        Nodes.add(in, out);
    }

    Nodes.print();
    //Nodes.Junction_points_print();
    Nodes.Bridges_print();

    return 0;
}
