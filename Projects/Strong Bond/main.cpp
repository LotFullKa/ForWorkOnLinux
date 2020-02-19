/*
 * 0 1              {0, 1, 2}
 * 1 2
 * 2 0
 * 3 4
 * 4 5
 * 5 6
 * 6 3
 * 7 8
 * 8 7
 * 8 1
 */

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
    queue <int>  FinishStuck;

    void prepair(){
        time = 0;
        colors.resize(colors.size(),0);
        while (!FinishStuck.empty())
            FinishStuck.pop();
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
        while(!FinishStuck.empty()) {
            int classteritem = FinishStuck.front();
            Classters[Classters.size() - 1].push_back(classteritem);
            FinishStuck.pop();
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

    void print(){
        cout << endl;
        for (set <int> i : graph) {
            for (int g : i) cout << g << "  ";
            cout << endl;
        }
        cout << endl;
    }

    void finishprint(){
        for(int i = 0; i < graph.size(); i++)
            DFS(i);
        for (int i = 0; i < graph.size(); ++i) {
            cout << FinishStuck.front() << "  ";
            FinishStuck.pop();
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
        FinishStuck.push(s);
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
        FinishStuck.push(s);
    }
};

int main() {
    Graphs Cities;

    int in, out;

    while(cin >> in >> out){
        if (in == -1) break;
        Cities.add(in, out);
    }

    Cities.print();

    Cities.finishprint();

    Cities.classtorBuildeer();

    Cities.classtorPrint();
    return 0;
}
