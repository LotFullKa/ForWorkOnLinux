/* Джон Макклейн сообщает по рации новую информацию о террористах в отдел с n полицейскими. Он звонит нескольким
 * сотрудникам и просит распространить информацию по отделу, зная, что у каждого полицейского есть связь с
 * определёнными коллегами. Джон Макклейн хочет, чтобы операция прошла успешно. Но если полицейский позвонит коллеге,
 * от которого(возможно, не напрямую) сам получил информацию, террористы смогут отследить этот звонок и помешать операции.
 * Если же двое сотрудников оповестят одного, ничего плохого не произойдёт. Помогите Джону Макклейну.
 * Выведите NO, если кто-то из полицейских ошибётся, делая звонок. Если всё пройдёт хорошо, выведите YES и порядок,
 * в котором полицейские получат информацию, считая, что полицейские оповещают коллег по возрастанию их номеров,
 * а в начале Джон даёт информацию тем, кому не может позвонить никто из коллег.

 * Формат ввода
 * Первая строка содержит пару целых чисел n и m (1 ≤ n, m ≤ 105) — количество вершин и рёбер заданного
 * ориентированного графа.

 * Следующие m строк содержат описания рёбер, по одному в строке. Каждая из строк содержит по два целых числа
 * a и b (0 ≤ a, b < n) — начало и конец ребра.

 * Формат вывода
 * Если такого порядка не существует, то выведите «NO» (без кавычек).

 * В противном случае, выведите «YES» в первую из строк. Во вторую строку выведите перестановку индексов вершин
 * (целые числа от 0 до n - 1), такую, что для каждого ребра его стартовая вершина находится строго левее его конечной вершины.
*/

#include <iostream>
#include <set>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

class Graph {
public:

    explicit Graph(int N){
        resize(N);
    }

    void add(int from, int to){
        if (max(from, to) + 1 > graph.size())
            resize(max(from, to) + 1);
        graph[from].insert(to);
        help_graph[to].insert(from);
    }

    void print(){

        int max = 0;
        for (const set<int>& i : help_graph)
            if (max < i.size()) max = i.size();

        int k = 0;
        cout << endl;
        for (const set <int>& i : graph) {
            for (int l = 0; l < (max - help_graph[k].size()); ++l) cout << "  ";
            for (int g : help_graph[k]) cout << g << " ";
            cout << ":" << k++ << ": ";
            for (int g : i) cout << g << "  ";
            cout << endl;
        }


    }

    void pathfinder(){
        if (circle_finder()){
            cout << "YES" << endl;

            for (int i = 0; !finish_stack.empty(); ++i) {
                cout << finish_stack.top() << "  ";
                finish_stack.pop();
            }
        } else {
            cout << "NO" << endl;
        }
    }

private:

    vector<set<int>>  graph;
    vector<set<int>>  help_graph;

    vector<int> colors;
    stack<int> finish_stack;


    void resize(int N){
        graph.resize(N);
        help_graph.resize(N);
        colors.assign(N,0);
    }

    void prepare(){
        colors.assign(graph.size(), 0);
    }

    bool circle_finder(){
        bool flag = false;
        for (int i = 0; i < graph.size(); ++i) {
            if (help_graph[i].empty()) {
                flag = true;
                if (!DFS(i))
                    return false;
            }
        }
        return flag;
    }

    bool DFS(int s){

        bool flag;
        if (colors[s] == 1)
            return false;
        if (colors[s] == 2)
            return true;

        colors[s] = 1;

        for (auto i : graph[s])
            if (!DFS(i) or i == s)
                return false;

        colors[s] = 2;
        finish_stack.push(s);
        return true;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    Graph Policeman(n);

    int in, out;



    for (int i = 0; i < m; ++i) {
        cin >> in >> out;
        Policeman.add(in, out);
    }

    //Policeman.print();

    Policeman.pathfinder();

    return 0;
}
