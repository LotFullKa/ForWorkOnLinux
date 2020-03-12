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
