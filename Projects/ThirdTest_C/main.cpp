/*
 * Вам дан неориентированный граф, состоящий из n вершин. На каждой вершине записано число; число, записанное на вершине i, равно ai. Изначально в графе нет ни одного ребра.

Вы можете добавлять ребра в граф за определенную стоимость. За добавление ребра между вершинами x и y надо заплатить ax + ay монет. Также существует m специальных предложений, каждое из которых характеризуется тремя числами x, y и w, и означает, что можно добавить ребро между вершинами x и y за w монет. Эти специальные предложения не обязательно использовать: если существует такая пара вершин x и y, такая, что для нее существует специальное предложение, можно все равно добавить ребро между ними за ax + ay монет.

Сколько монет минимально вам потребуется, чтобы сделать граф связным? Граф является связным, если от каждой вершины можно добраться до любой другой вершины, используя только ребра этого графа.

Формат ввода
В первой строке заданы два целых числа n и m (1 ≤ n ≤ 2 ⋅ 105, 0 ≤ m ≤ 2 ⋅ 105) — количество вершин в графе и специальных предложений, соответственно.

Во второй строке заданы n целых чисел  (1 ≤ ai ≤ 1012) — числа, записанные на вершинах.

Затем следуют m строк, в каждой из которых заданы три целых числа x, y и w (1 ≤ x, y ≤ n, 1 ≤ w ≤ 1012, x ≠ y), обозначающие спецпредложение: можно добавить ребро между вершинами x и y за w монет.

Формат вывода
Выведите одно целое число — минимальное количество монет, которое необходимо потратить, чтобы сделать граф связным.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <set>

using namespace std;

class Tree {
public:
    Tree(long long int n) {
        parent.resize(n);
    }

    void add (const long long int &a) {
        prices.push_back(a);
    }

    void make_edges(long long int min_i){
        for (long long int i = 0; i < prices.size(); ++i) {
            if (i != min_i)
                edges.emplace_back(i, min_i, prices[i] + prices[min_i]);
        }
    }

    void add_special(const long long int &x, const long long int &y, const long long int &w){
        edges.emplace_back(x, y, w);
    }

    long long int get_weight_MST(){
        return Kruskal_al();
    }

private:
    struct Edge {
        Edge(const long long int &b, const long long int &e, const long long int &w): b(b), e(e), w(w) {}

        long long int b;
        long long int e;
        long long int w;

    };

    static bool compare(const Edge &a, const Edge &b) {
        return a.w > b.w;
    }

    vector<long long int > parent;
    vector<long long int > prices;
    vector<Edge> edges;


    long long int Kruskal_al(){
        for (long long int i = 0; i < parent.size(); ++i) {
            parent[i] = i;
        }

        priority_queue<Edge, vector<Edge>, function<bool(Edge, Edge)> > q(compare, edges);

        long long int weight_sum = 0;

        long long int n = 0;
        while (!q.empty() and n < prices.size() - 1) {
            Edge edge = q.top();
            q.pop();

            if (find_set(edge.b) != find_set(edge.e)){
                weight_sum += edge.w;
                n++;
                union_sets(edge.b,edge.e);
            }
        }

        return weight_sum;
    }

    long long int find_set (long long int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find_set (parent[v]);
    }

    void union_sets (long long int a, long long int b) {
        a = find_set (a);
        b = find_set (b);
        if (a != b)
            parent[b] = a;
    }

};


int main() {
    long long int n, m;
    std::cin >> n >> m;

    Tree my_little_tree(n);
    long long int x, y, w;
    long long int min = -1, k;
    for (long long int i = 0; i < n; ++i) {
        cin >> x;
        my_little_tree.add(x);
        if (min == -1 or x < min) {
            min = x;
            k = i;
        }
    }
    my_little_tree.make_edges(k);
    for (long long int i = 0; i < m; ++i) {
        std::cin >> x >> y >> w;
        my_little_tree.add_special(x - 1, y - 1, w);
    }

    cout << my_little_tree.get_weight_MST();
    return 0;
}
