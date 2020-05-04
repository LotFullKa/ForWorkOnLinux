#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <assert.h>

using namespace std;

class Graphs{
private:

    int time = 1;




    vector<set <int>>  graph;              // под цифрой лежат цифры куда можной пойти
    vector<set <int>>  regraph;
    vector<int> colors;                     // 0 - белый   1 - серый    2 - черный
    vector<int> TimeIn;
    vector<int> TimeOut;
    queue <int>  FinishStack;


    void resize(int N){
        regraph.resize(N);
        graph.resize(N);
        colors.resize(N, 0);
        TimeIn.resize(N);
        TimeOut.resize(N);
    }

    void prepare(){
        while (!FinishStack.empty()){
            FinishStack.pop();
        }

        colors.assign(colors.size(), 0);

        time = 0;
    }

    void DFS(int s, vector<set<int>> & graphToRun) {
        if(colors[s] != 0) return;
        colors[s] = 1;
        TimeIn[s] = time;
        time ++;

        for (auto i : graphToRun[s]) DFS(i, graphToRun);

        TimeOut[s] = time;
        time++;
        colors[s] = 2;
        FinishStack.push(s);
    }

    void collectorGraph (vector<set<int>>& components, vector<set<int>>& recomponents, int &counter){
        if (components.size() == 1) return;

        for(int i = 0; i < components.size(); i++)
            DFS(i, components);
        prepare();

        vector<set<int>> temp = classtorBuildeer(recomponents);
        //вернул пару графа и  реграфа (от кластера, который он построил от графа и реграфа что пришли ....гыг)
        pair<vector<set<int>>, vector<set<int>>> MyGraphs = clusterWooper(temp);

        components = MyGraphs.first;
        recomponents = MyGraphs.second;

        ++counter;

        collectorGraph(components, recomponents, counter);
    }

    // делает из кластеров(компонент сильной связанности) подвешенный граф, а потом дополняет их до цикла,
    // теперь это снова граф, причем с одним циклом
     pair<vector<set<int>>, vector<set<int>>> clusterWooper(vector<set<int>>& Classters){
        vector<set<int>> components(Classters.size());
        vector<set<int>> recomponents(Classters.size());
        for (int i = 0; i < Classters.size(); ++i) {
            for (int child : Classters[i]){
                for (int friends : graph[child]){
                    for (int g = 0; g < Classters.size(); ++g){
                        if (Classters[g].count(friends) != 0 and i != g){
                            components[i].insert(g);
                            recomponents[g].insert(i);
                        }
                    }
                }
            }
        }

        //добовляет цикл в подвешенный граф
        bridgeBuilder(components, recomponents);
        return pair<vector<set<int>>, vector<set<int>>>(components, recomponents);
    }

    // находит корень в подвешенном граффе
    static int  rootSicker(const vector<set<int>> &recomponents){
        for (int i = 0; i < recomponents.size(); ++i) {
            if (recomponents[i].empty())
                return i;
        }
        return -1;
    }

    // в подвешенном графе находит ссамое адльную ноду от startPoint (рекурсивная)
    static int diver(const vector<set<int>>& components, int startPoint, vector<int>& deep){

        for (int i = 0; i < components.size(); ++i){
            if(!components[i].empty()) {
                deep[i] += 1;
                diver(components, i, deep);
            }
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

    //рекурсивно строит клаассторы ! С условие что по графу прошлись DFS и заполнили ТаймИн
    vector<set<int>> classtorBuildeer(vector<set<int>> &reverseGraph){
        vector<set<int>> Classters;
        int istarer = 0;
        int max = TimeOut[0];

        for (int i = 0; i < TimeOut.size(); ++i) {
            if(max < TimeOut[i]){
                max = TimeOut[i];
                istarer = i;
            }
        }

        if(max == 0) return Classters;

        assert(FinishStack.empty());

        DFS(istarer, reverseGraph);

        Classters.resize(Classters.size() + 1);
        while(!FinishStack.empty()) {
            int classteritem = FinishStack.front();
            Classters[Classters.size() - 1].insert(classteritem);
            FinishStack.pop();
            TimeOut[classteritem] = 0;
        }

        for (set<int> piece : classtorBuildeer(reverseGraph))
            Classters.push_back(piece);
        return Classters;
    }

public:

    Graphs() = default;

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
        vector<set<int>> Classters = classtorBuildeer(regraph);
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

};

int main() {
    Graphs Cities;

    int in, out, n, m;

    cin >> n >> m;

    for (int i = 0; i < m; ++i){
        cin >> in >> out;
        Cities.add(in , out);
    }

    Cities.print();

    Cities.finishprint();

    Cities.classtorPrint();

    //cout << Cities.MakeYourGraphSimpleAgain();

    return 0;
}
