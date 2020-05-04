/*
 * Фрэнку опять прилетел новый заказ. Однако в этот раз город играет по очень странным правилам:
 * все дороги в городе односторонние и связывают только офисы нанимателей перевозчика.
 * Множество офисов, между любыми двумя из которых существует путь, образуют квартал, если нельзя добавить
 * никакие другие, чтобы условие выполнялось. Фрэнку интересно, каково минимальное количество односторонних дорог
 * нужно ещё построить, чтобы весь город стал квраталом.
 *
 * Формат ввода
 * Первая строка содержит пару целых чисел n и m (1 ≤ n ≤ 105, 0 ≤ m ≤ 105) — количество вершин и рёбер
 * заданного ориентированного графа.
 *
 * Следующие m строк содержат описания рёбер, по одному в строке.
 * Каждая из строк содержит по два целых числа a и b (1 ≤ a, b ≤ n) — начало и конец ребра.
 *
 * Формат вывода
 * Выведите одно число — количество дорог, которые нужно добавить, чтобы весь город стал одним кварталом.
 */

#include <iostream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

class Graph{
public:

    Graph() = default;

    Graph(int N){
        resize(N);
    }

    //добавляем ребра в граф
    void add(int from, int to){

        if(from == to) return;

        if (max(from, to) >= graph.size())
            resize(max(from, to));

        graph[from].insert(to);
        regraph[to].insert(from);
    }

    int MakeYourGraphSimpleAgain(){
        return collector();
    }

private:

    //глобальные контейнеры
    vector<set<int>> graph;
    vector<set<int>> regraph;
    vector<int> colors;                 // 0 - белый 1 - серый ( выполняется ) 2 - черный ( был пройден )
    stack<int> TopSorted;

    void resize(int N){
        graph.resize(N);
        regraph.resize(N);
        colors.resize(N);
    }

    //special add       I`m SO  special !!!
    static void add(int from, int to, vector<set<int>>& components, vector<set<int>>& recomponents){

        if(from == to) return;

        components[from].insert(to);
        recomponents[to].insert(from);
    }

    //обход без захвата ребер
    void DFS (int vertex){
        if (colors[vertex] != 0) return;
        colors[vertex] = 1;

        for (auto friends : graph[vertex]){
            DFS(friends);
        }

        TopSorted.push(vertex);
        colors[vertex] = 2;
    }

    //обход вместе с захватом ребер ведущих в другую компоненту
    void DFS (int vertex, const vector<set<int>>& components, int color){
        if (colors[vertex] != 0) return;
        colors[vertex] = 1;

        for (auto friends : components[vertex]){
            DFS(friends, components, color);
        }

        colors[vertex] = color + 3;
    }

    void prepare(){

        colors.assign(colors.size(), 0);
    }

    //первый обход графа для записи времени выхода
    void TimeWriter(){
        prepare();
        for (int i = 0; i < graph.size(); i++)
            DFS(i);
    }

    int collector (){
        int clustersSize = 0;

        TimeWriter();

        clusterBuilder(clustersSize);

        condensation(clustersSize);

        if (clustersSize == 1) return  0;

        return max(rootSicker(), diver());
    }


    void clusterBuilder(int& clustersSize) {

        prepare();

        int color = -1;
        while (!TopSorted.empty()) {

            if (colors[TopSorted.top()] != 0) {
                TopSorted.pop();
                continue;
            }

            DFS(TopSorted.top(), regraph, ++color);
            TopSorted.pop();

            ++clustersSize;
        }
    }


    // конденсирует граф
    void condensation(int& clustersSize){
        vector<set<int>> temp(clustersSize);
        vector<set<int>> retemp(clustersSize);

        for (int from = 0; from < graph.size(); ++from)
            for (auto child : graph[from])
                if (colors[from] != colors[child])
                    add(colors[from] - 3, colors[child] - 3, temp, retemp);

        graph = temp;
        regraph = retemp;
    }



    //вернет количество начальных ущлов
    int rootSicker(){
        int count = 0;
        for (const auto & recomponent : regraph){
            if (recomponent.empty()) ++count;
        }
        return count;
    }

    //вернет количество тупиковых узлов
    int diver(){
        int count = 0;
        for (const auto & component : graph){
            if (component.empty()) ++count;
        }
        return count;
    }
};

int main() {
    int M, N, from, to;

    cin >> N >> M;

    Graph Streets(N);

    for (int i = 0; i < M; ++i) {
        cin >> from >> to;
        Streets.add (from - 1, to - 1);
    }

    cout << Streets.MakeYourGraphSimpleAgain();
}
