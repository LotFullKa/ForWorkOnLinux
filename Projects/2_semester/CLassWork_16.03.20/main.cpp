#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;

class Suspended_graph{
public:

    explicit Suspended_graph(int N){
        resize(N);
    }

    void add(int from, int to, int wgt){
        graph[from].insert(to);
        graph[to].insert(from);
        weight[from][to] = wgt;
        weight[to][from] = wgt;
    }

    void print(int startPoint){

        DBFS(startPoint);

        for (int i = 0; i < depth.size(); ++i) {
            cout << i << "| " << depth[i];
        }
    }

private:
    vector<vector<int>> weight;
    vector<set<int>> graph;
    deque<int> queue;
    vector<int> depth;

    void resize(int N){
        graph.resize(N);
        weight.resize(N);
        for(auto& s : weight) s.resize(N, -2);
        depth.resize(N, -1);
    }

    void DBFS(int startPoint) {
        int vertex;
        queue.push_back(startPoint);

        while (!queue.empty()) {
            vertex = queue.front();
            queue.pop_front();
            for (auto son: graph[vertex]) {
                if (weight[vertex][son] < depth[son] or depth[son] == -1)
                    depth[son] = weight[vertex][son];
            }

            for (auto son: graph[vertex]) {
                if (weight[vertex][son] <= depth[son]) {
                    if (weight[vertex][son] == 0)
                        queue.push_front(son);
                    if (weight[vertex][son] == 1)
                        queue.push_back(son);
                }
            }
        }
    }
};

int main() {
    int m, n, from, to, wgt;
    cin >> m >> n;

    Suspended_graph graph(n);

    for (int i = 0; i < m; ++i) {
        cin >> from >> to >> wgt;
        graph.add(from, to , wgt);
    }

    graph.print(0);


    return 0;
}
