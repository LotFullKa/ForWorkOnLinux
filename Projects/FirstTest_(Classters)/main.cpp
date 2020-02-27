#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

class Graphs{
private:

    int time = 1;

    vector<vector<int>> Classters;
    vector<set <int>>  graph;              // под цифрой лежат цифры куда можной пойти
    vector<set <int>>  regraph;
    vector<int> colors;                     // 0 - белый   1 - серый    2 - черный
    vector<int> recolors;
    vector<int> TimeIn;
    vector<int> reTimeIn;
    vector<int> TimeOut;
    vector<int> reTimeOut;
    queue <int>  FinishStack;

    void prepair(){
        time = 0;
        colors.resize(colors.size(),0);
        while (!FinishStack.empty())
            FinishStack.pop();
    }

    void resize(int N){
        regraph.resize(N);
        graph.resize(N);
        colors.resize(N, 0);
        recolors.resize(N, 0);
        TimeIn.resize(N);
        reTimeIn.resize(N);
        TimeOut.resize(N);
        reTimeOut.resize(N);
    }

public:

    Graphs() = default;

    void add(int from, int to){
        if (max(from, to) + 1 > graph.size())
            resize(max(from, to) + 1);
        graph[from].insert(to);
        regraph[to].insert(from);
    }

    void classtorBuildeer(){
        int istarer = 0;
        int max = TimeOut[0];

        for (int i = 0; i < TimeOut.size(); ++i) {
            if(max < TimeOut[i]){
                max = TimeOut[i];
                istarer = i;
            }
        }

        if(max == 0) return;

        prepair();

        reDFS(istarer);

        Classters.resize(Classters.size() + 1);
        while(!FinishStack.empty()) {
            int classteritem = FinishStack.front();
            Classters[Classters.size() - 1].push_back(classteritem);
            FinishStack.pop();
            TimeOut[classteritem] = 0;
        }

        classtorBuildeer();
    }

    void classtorPrint(){
        for(int i = 0; i < Classters.size(); ++i){
            cout << " " << i <<" : { ";
            for (int g : Classters[i]) {
                cout << g << " ";
            }
            cout << "}" << endl;
        }
    }

    void countClasstersPrint(){
        cout << Classters.size() << endl;
    }

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

    void finishprint(){
        for(int i = 0; i < graph.size(); i++)
            DFS(i);
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
    }

    void DFS(int s) {
        if(colors[s] != 0) return;
        colors[s] = 1;
        TimeIn[s] = time;
        time ++;

        for (auto i : graph[s]) DFS(i);

        TimeOut[s] = time;
        time++;
        colors[s] = 2;
        FinishStack.push(s);
    }

    void reDFS(int s) {
        if(recolors[s] != 0) return;
        recolors[s] = 1;
        reTimeIn[s] = time;
        time ++;

        for (auto i : regraph[s]) reDFS(i);

        reTimeOut[s] = time;
        time++;
        recolors[s] = 2;
        FinishStack.push(s);
    }
};

int main() {
    Graphs Cities;

    int in, out, n, m;

    cin >> n >> m;

    for (int i = 0; i < m; ++i){
        cin >> in >> out;
        Cities.add(in, out);
    }

    Cities.print();

    Cities.finishprint();

    Cities.classtorBuildeer();

    Cities.classtorPrint();

    Cities.countClasstersPrint();
    return 0;
}
