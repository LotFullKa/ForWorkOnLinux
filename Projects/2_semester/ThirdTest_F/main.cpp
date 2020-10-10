/*
 * Шрек и Фиона пригласили всех своих друзей на свою свадьбу.
 * На церемонии они хотят рассадить их всех на две непустые части так, чтобы количество
 * знакомств между двумя частями было минимальным. Всего приглашенных на свадьбу n, а каждое знакомство обоюдно.

Вам дан граф, в котором ребро означает знакомство между людьми.
 Помогите Шреку и Фионе поделить гостей на две непустые части.

Формат ввода
В первой строке входного файла записано целое число n () — число гостей.
 Каждая из следующих n строк содержит по n символов. i-й символ j-й из этих строк равен «1»,
 если между вершинами i и j есть ребро, и «0» в противном случае. Заданная таким образом матрица
 смежности является антирефлексивной (на главной диагонали стоят нули) и симметричной (относительно главной диагонали).

Формат вывода
Выведите в выходной файл две строки. На первой выведите номера вершин, попавших в левую часть,
через пробел, а на второй — номера вершин, попавших в правую часть, также через пробел.
 Номера вершин можно выводить в любом порядке.

 */

#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Guests{
public:
    Guests(int &n) : n(n) {
        graph.resize(n);
        for (vector<int> & i : graph){
            i.resize(n,0);
        }
    }

    void add(const int &x, const int &y) {
        graph[x][y] = 1;
    }

    void check(){
        mincut();
        set <int> help_set;
        for (auto i : best_cut) {
            cout << i + 1<< " ";
            help_set.insert(i);
        }
        cout << endl;
        for (int i = 0; i < n; ++i) {
            if (!help_set.count(i))
                cout << i + 1 << " ";
        }
    }

private:
    vector< vector<int> > graph;
    int best_cost = -1;
    vector<int> best_cut;
    int n;

    void mincut() {
        vector< vector<int> > v(n);
        for (int i = 0; i < n; ++i) {
            v[i].assign(1, i);
        }
        vector<int> weight(n);
        vector<bool> exist(n, true);
        vector<int> in_a(n);
        for (int ph = 0; ph < n - 1; ++ph) {

            in_a.assign(in_a.size(), false);
            weight.assign(weight.size(), 0);
            for (int it = 0, prev = 0; it < n - ph; ++it) {
                int sel = -1;
                for (int i = 0; i < n; ++i) {
                    if (exist[i] && !in_a[i] && (sel == -1 || weight[i] > weight[sel]))
                        sel = i;
                }
                if (it == n - ph - 1) {
                    if (weight[sel] < best_cost || best_cost == -1){
                        best_cost = weight[sel];
                        best_cut = v[sel];
                    }
                    v[prev].insert( v[prev].end(), v[sel].begin(), v[sel].end());
                    for (int i = 0; i < n; ++i) {
                        graph[prev][i] = graph[i][prev] += graph[sel][i];
                    }
                    exist[sel] = false;
                } else {
                    in_a[sel] = true;
                    for (int i = 0; i < n; ++i)
                        weight[i] += graph[sel][i];
                    prev = sel;
                }
            }
        }
    }

};

int main() {
    int n;
    cin >> n;

    Guests guests(n);

    string str;
    for (int i = 0; i < n; ++i) {
        cin >> str;
        for (int g = 0; g < n; ++g) {
            if (str[g] == '1')
                guests.add(i, g);
        }
    }

    guests.check();

    return 0;
}
