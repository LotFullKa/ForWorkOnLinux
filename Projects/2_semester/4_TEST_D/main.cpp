/*
 * Задано дерево с корнем, содержащее (1 ≤ n ≤ 100 000) вершин, пронумерованных от 0 до n-1.
Требуется ответить на m (1 ≤ m ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин.
Запросы генерируются следующим образом. Заданы числа a1, a2 и числа x, y и z.
Числа a3, ..., a2m генерируются следующим образом: ai = (x ⋅ ai-2 + y ⋅ ai-1 + z) mod n.
Первый запрос имеет вид (a1, a2). Если ответ на i-1-й запрос равен v, то i-й запрос имеет вид ((a2i-1 + v) mod n, a2i).

Для решения задачи можно использовать метод двоичного подъёма.

Формат ввода
Первая строка содержит два числа: n и m.
Корень дерева имеет номер 0.
Вторая строка содержит n-1 целых чисел, i-е из этих чисел равно номеру родителя вершины i.
Третья строка содержит два целых числа в диапазоне от 0 до n-1: a1 и a2.
Четвертая строка содержит три целых числа: x, y и z, эти числа неотрицательны и не превосходят 109.

Формат вывода
Выведите в выходной файл сумму номеров вершин — ответов на все запросы.
 */

#include <iostream>
#include <vector>

using namespace std;

class LCA {
public:
    LCA(vector< vector<int> >& graph, int size, long long int log_size) :
        size(size), log_size(log_size), graph(graph) {

        time_in.resize(size, 0);
        time_out.resize(size, 0);

        parents.resize(size, vector<int>(log_size + 1));

        dfs(0, 0);
    }

    void start_parameters(int a1, int a2, int x, int y, int z) {
        this->a1 = a1;
        this->a2 = a2;
        this->x = x;
        this->y = y;
        this->z = z;
    }

    int query(int result) {

        pair<int, int> memory = make_pair((a1 + result) % size, a2);

        a1 = (x * a1 + y * a2 + z) % size;
        a2 = (x * a2 + y * a1 + z) % size;

        return query(memory.first, memory.second);
    }

private:
    vector< vector<int> > graph;
    vector< vector<int> > parents;
    vector<int> time_in;
    vector<int> time_out;

    int size;
    int now_time = 0;
    long long int log_size;
    long long int a1, a2, x, y, z;

    void dfs(int vertex, int parent) {
        time_in[vertex] = ++now_time;
        parents[vertex][0] = parent;

        for (int parent_id = 1; parent_id < log_size; ++parent_id) {
            parents[vertex][parent_id] = parents[parents[vertex][parent_id - 1]][parent_id - 1];
        }

        for (auto& child : graph[vertex]) {
            if (child != parent)
                dfs(child, vertex);
        }

        time_out[vertex] = ++now_time;
    }

    int query(int left, int right) {
        if (is_child(left, right))
            return left;

        if (is_child(right, left))
            return right;

        for (int node = log_size; node >= 0; --node) {
            if(!is_child(parents[left][node], right))
                left = parents[left][node];
        }

        return parents[left][0];
    }

    bool is_child (int parent, int son) {
        return time_in[parent] <= time_in[son]  and time_out[parent] >= time_out[son];
    }
};

int main() {
    int N, m;

    cin >> N >> m;

    vector< vector<int> > parents;
    parents.resize(N);
    int buffer;

    for (int ver = 1; ver < N; ++ver) {
        cin >> buffer;
        parents[buffer].emplace_back(ver);
    }

    int log_size = 1;
    for (; (1 << log_size) <= N; ++log_size);

    LCA tree(parents, N, log_size);

    int a1, a2, x, y, z;
    cin >> a1 >> a2 >> x >> y >> z;

    tree.start_parameters(a1, a2, x, y, z);

    long long int answer = 0,
        sum = 0;

    for (int i = 0; i < m; ++i) {
        answer = tree.query(answer);
        sum += answer;
    }

    cout << sum;

    return 0;
}
