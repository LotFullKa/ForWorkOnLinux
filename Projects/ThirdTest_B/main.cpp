/*
 * Формат ввода
 * Первая строка входного файла содержит два натуральных числа n и m — количество вершин и ребер графа соответственно.
 * Следующие m строк содержат описание ребер по одному на строке. Ребро номер i описывается тремя
 * натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100 000).
 * n ≤ 20 000, m ≤ 100 000.
 *
 * Граф является связным.
 * Формат вывода
 * Первая строка выходного файла должна содержать одно натуральное число — вес минимального остовного дерева.
 */

#include <iostream>
#include <functional>
#include <vector>
#include <queue>

using namespace std;

class Tree {
public:
    Tree(int n) {
        parent.resize(n);
    }

    void add (const int &b, const int &e, const int &w) {
        edges.emplace_back(b, e, w);
    }

    int get_weight_MST(){
        return Kruskal_al();
    }

private:
    struct Edge {
    Edge(const int &b, const int &e, const int &w): b(b), e(e), w(w) {}

    int b;
    int e;
    int w;
};

static bool compare(const Edge &a, const Edge &b) {
    return a.w > b.w;
}

vector<int> parent;
vector<Edge> edges;

int Kruskal_al(){
    for (int i = 0; i < parent.size(); ++i) {
        parent[i] = i;
    }
    priority_queue<Edge, vector<Edge>, function<bool(Edge, Edge)> > q(compare, edges);
    int weight_sum = 0;

    while (!q.empty()) {
        Edge edge = q.top();
        q.pop();

       if (find_set(edge.b) != find_set(edge.e)){
           weight_sum += edge.w;
           union_sets(edge.b,edge.e);
       }
    }

    return weight_sum;
}

int find_set (int v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set (parent[v]);
}

void union_sets (int a, int b) {
    a = find_set (a);
    b = find_set (b);
    if (a != b)
        parent[b] = a;
}

};

int main() {
    int n, m;
    cin >> n >> m;
    Tree my_little_tree(n);

    int b, e, w;
    for (int i = 0; i < m; ++i) {
        cin >> b >> e >> w;
        my_little_tree.add(b - 1, e - 1, w);
    }

    cout  << my_little_tree.get_weight_MST();
    return 0;
}
