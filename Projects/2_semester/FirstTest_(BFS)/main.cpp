
/*
 * Леон и Матильда собрались пойти в магазин за молоком. Их хочет поймать Стэнсфилд, поэтому нашим товарищам нужно
 * сделать это как можно быстрее. Каково минимальное количество улиц, по которым пройдёт хотя бы один из ребят
 * (либо Матильда, либо Леон, либо оба вместе)?

 * Формат ввода
 * Первая строка содержит количество вершин n (1 ≤ n ≤ 105), количество ребер m (0 ≤ m ≤ 105) и номера вершин
 * графа leon, matilda, milk, в которых находятся соответственно Леон, Матильда и магазин с молоком.

 * Следующие m строк содержат ребра графа. В каждой строке два числа, разделенные пробелом,
 * если между i и j существует ребро. Гарантируется, что в графе нет петель и мультиребер.
 */

#include <iostream>
#include <queue>
#include <set>

using namespace std;

class Graph {
public:

    explicit Graph(int N){
        resize(N + 1);
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
            cout << "  "<< k++ << ": ";
             for (int g : i) cout << g << "  ";
            cout << endl;
        }
    }

    int pathfinder(int Leon, int Matilda, int Milk){
        BFS(Leon, Leons_distance);
        BFS(Matilda, Matildas_distance);
        BFS(Milk, Milks_distance);

        int min = -1;

        for(int i = 1; i < graph.size(); ++i){
            int total_distance = Milks_distance[i] + Matildas_distance[i] + Leons_distance[i];
            if (total_distance < min or min == -1)
                min = total_distance;
        }
        return min;
    }

private:

    vector<set<int>>  graph;

    vector<int> Leons_distance;
    vector<int> Matildas_distance;
    vector<int> Milks_distance;

    vector<int> colors;


    void resize(int N){
        graph.resize(N);
        colors.assign(N,0);
    }

    void prepare(){
        colors.assign(graph.size(), 0);
    }

    void BFS(int start, vector<int>& distance) {
        queue<int> qu;

        prepare();
        distance.resize(graph.size(), 0);
        qu.push(start);
        int now_node;
        while (!qu.empty()){
            now_node = qu.front();
            qu.pop();
            colors[now_node] = 1;
            for (int i : graph[now_node]) {
                if (colors[i] == 1)
                    continue;
                if (distance[i] == 0){
                    distance[i] = distance[now_node] + 1;
                    qu.push(i);
                }
            }
        }
        }
};

int main() {

    int N, m, Leon, Matilda, Milk;
    cin >> N >>  m >> Leon >> Matilda >> Milk;

    Graph streets(N);

    int from, to;
    for (int i = 0; i < m; ++i) {
        cin >> from >> to;
        streets.add(from, to);
    }

    //streets.print();

    cout << streets.pathfinder(Leon, Matilda, Milk) << endl;
    return 0;
}
