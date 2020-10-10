/*
 * Формат ввода
 * Первая строка входного файла содержит два натуральных числа n и m — количество вершин и ребер графа соответственно.
 * Следующие m строк содержат описание ребер по одному на строке.
 * Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его
 * вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100 000). n ≤ 20 000, m ≤ 100 000.
 *
 * Граф является связным.
 *
 * Формат вывода
 * Первая строка выходного файла должна содержать одно натуральное число — вес минимального остовного дерева.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <set>

using namespace std;

class SpanningTree {
public:
    explicit SpanningTree(int n) {
        graph.resize(n);
        used.resize(n, false);
    }

    void add(const int &start, const int &finish, const int &weight) {
        graph[start].insert(make_pair(finish, weight));
        graph[finish].insert(make_pair(start, weight));
    }

    int get_weight_MST(){
        return weight_MST();
    }

private:

    static bool compare (const pair<int, int> &a, const pair<int, int> &b){
        return a.second > b.second;
    }

    vector<set<pair<int, int> > > graph;
    vector<bool> used;

    int weight_MST() {
        priority_queue<pair<int, int>, vector<pair<int, int> >, function<bool(const pair<int, int>, const pair<int, int>)> > q (compare);
        int weight_sum = 0;

        q.push(make_pair(0,0));

        int weight, vertex;
        while(!q.empty()){
            weight = q.top().second;
            vertex = q.top().first;
            q.pop();

            if (!used[vertex]){

                used[vertex] = true;
                weight_sum += weight;

                for (pair<int, int> edge: graph[vertex]) {
                    if(!used[edge.first])
                        q.push(edge);
                }
            }
        }

        return weight_sum;
    }

};

int main() {
    int n, m;
    std::cin >> n >> m;

    SpanningTree my_little_tree(n);

    int b, e, w;
    for (int i = 0; i < m; ++i) {
        std::cin >> b >> e >> w;
        my_little_tree.add(b - 1, e - 1, w);
    }

    cout << my_little_tree.get_weight_MST();
    return 0;
}
