/*
 * Шрек и Осёл уже были на пути домой. Им оставалось только преодолеть лес, который отделял их от болота.
 * Но они поссорились, поэтому не хотят идти вместе.

Лес представляет собой опушки, пронумерованы числами от 1 до n и соединенные m дорожками
 (может быть несколько дорожек соединяющих две опушки, могут быть дорожки, соединяющие опушку с собой же).
 Из-за ссоры, если по дорожке прошел один из друзей, то второй по той же дорожке уже идти не может.
 Сейчас друзья находятся на опушке с номером s, а болото Шрека — на опушке с номером t. Помогите Шреку и Ослу добраться до болота.

Формат ввода
В первой строке файла записаны четыре целых числа — n, m, s и t (2 ≤ n ≤ 105, 0 ≤ m ≤ 105, ).
В следующих m строках записаны пары чисел. Пара чисел (x, y) означает,
 что есть дорожка с опушки x до опушки y (из-за особенностей местности дорожки односторонние).

Формат вывода
Если существует решение, то выведите YES и на двух отдельных строчках сначала последовательность опушек для Шрека,
 затем путь для Осла.

Если решения не существует, выведите NO.

Если решений несколько, выведите любое.
 */

#include <iostream>
#include <vector>

using namespace std;

class Network{
public:
    Network (const int &n,const int &sour,const int &targ) : sour(sour), targ(targ){
        graph.resize(n);
        visited.resize(n);
    };

    void add(int x, int y){
//        for (Edge& edge : graph[x]) {
//            if (edge.end == y and edge.straight) {
//                edge.capacities += 1;
//                back_edge(edge).capacities += 1;
//                return;
//            }
//        }
        graph[x].emplace_back(x, y, true);
        graph[y].emplace_back(y, x, false);
    }


    void ways(){
        visited.assign(visited.size(), false);
        if (dfs(sour, 1)) {
            visited.assign(visited.size(), false);
            if (dfs(sour, 1)) {
                print_answer(1);
                return;
            }
        }

        print_answer(0);
    }

    void print_answer(const bool& flag){
        bool fst_str = true;
        if (flag){
            cout << "YES" << endl;
            dfs_for_answer(sour);
            dfs_for_answer(sour);
            for (int i = answer.size() - 1; i > -1; --i){
                cout << answer[i] + 1 << " ";
                if (fst_str and answer[i] == targ){
                    cout << endl;
                    fst_str = false;
                }
            }

        } else {
            cout << "NO";
        }
    }
private:

    struct Edge{

        Edge(int &begin, int &end, bool st) :
            begin(begin),
            end(end),
            straight(st) {}

        int begin;
        int end;
        int flow = 0;
        int capacities = 1;
        bool straight;
    };

    Edge& back_edge(Edge& me){
        for (Edge& edge : graph[me.end]){
            if (edge.end == me.begin)
                return edge;
        }
    }

    vector<vector<Edge> > graph;
    vector<bool> visited;
    vector<int> answer;
    int targ;
    int sour;

    int dfs(int ver, int Cmin) {
        if (ver == targ){
            return Cmin;
        }

        visited[ver] = true;
        for (Edge& u : graph[ver]) {
            if(!visited[u.end] and u.straight and (u.capacities - u.flow)) {
                int delta = dfs(u.end, min(Cmin, u.capacities - u.flow));
                if (delta > 0){
                    u.flow += delta;
                    back_edge(u).flow -= delta;
                    return delta;
                }
            }
        }

        if (ver != sour) {
            for (Edge &u : graph[ver]) {
                if (!visited[u.end] and (u.capacities - u.flow)) {
                    int delta = dfs(u.end, min(Cmin, u.capacities - u.flow));
                    if (delta > 0) {
                        u.flow += delta;
                        back_edge(u).flow -= delta;
                        return delta;
                    }
                }
            }
        }

        return 0;
    }

    bool dfs_for_answer(int& s) {
        if (s == targ) {
            answer.emplace_back(targ);
            return true;
        }

        for (Edge& u : graph[s])
            if (u.flow != 0 and u.straight) {
                if (dfs_for_answer(u.end)) {
                    answer.emplace_back(s);
                    u.flow -= 1;
                    return true;
                }
            }
        return false;
    }
};

int main() {
    int n , m, s, t;
    cin >> n >> m >> s >> t;
    Network network(n, s - 1, t - 1);
    int x, y;
    for (int i = 0; i < m; ++i) {
        cin >> x >> y;
        network.add(x - 1, y - 1);
    }

    network.ways();
    return 0;
}

/*
5 6 1 5
1 2
2 3
3 4
4 5
1 3
2 5
 */