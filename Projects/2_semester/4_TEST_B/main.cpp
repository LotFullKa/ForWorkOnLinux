/*
 * ООО "Новые Железные Дороги" поручило вам разработать систему бронирования
 * билетов на новый маршрут поезда дальнего следования. Маршрут проходит через N станций, занумерованных от 0 до N-1.
 * Вместимость поезда ограничена.
В систему бронирования последовательно приходят запросы от пассажиров с указанием номера начальной и конечной станции,
 а также количество билетов, которые пассажир хочет приобрести.
Требуемая скорость обработки каждого запроса - .

Формат ввода
Первая строка содержит число N –– количество станций (1 ≤ N ≤ 50 000).

Вторая строка содержит начальное количество проданных билетов.
В примере продано по одному билету на перегонах 0->1 и 4->5.

В третьей строке указана вместимость поезда. В четвёртой — количество запросов на бронирование.
Далее идут непосредственно запросы в виде: номер начальной станции, номер конечной станции, количество запрашиваемых билетов.

Формат вывода
Необходимо вывести номера запросов, которые не удалось выполнить из-за отсутствия свободных мест.
 */

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class Tree_Segments{
public:
    Tree_Segments( vector<int>& base): size(base.size()) {
        tree.assign(4 * size, make_pair(INT32_MAX, 0));

        build(base, 1, 0, size - 1);
    }

    bool check_it_out(const int &left, const int &right, const int &num){
        if (get(1, 0, size - 1, left, right) >= num) {
            update(1, 0, size - 1, left, right, num);
            return true;
        } else {
            return false;
        }
    }

private:

    vector<pair<int, int>> tree;
    int size;

    void build(vector<int>& base, int vertex, int left, int right) {
        if (left == right)
            tree[vertex].first = base[left];
        else {
            int mid = (left + right) / 2;

            build(base, vertex * 2, left, mid);
            build(base, vertex * 2 + 1, mid + 1, right);

            tree[vertex].first = std::min(tree[vertex * 2].first, tree[vertex * 2 + 1].first);
        }
    }

    void update(int vertex, int left, int right, int cl, int cr, int add) {

        if (cl > cr)
            return;

        if (cl == left and cr == right)
            tree[vertex].second += add;

        else {
            int mid = (left + right) / 2;

            update(vertex * 2, left, mid, cl, std::min(cr, mid), add);
            update(vertex * 2 + 1, mid + 1, right, std::max(cl, mid + 1), cr, add);

            tree[vertex].first = std::min(
                    tree[vertex * 2].first - tree[vertex * 2].second,
                    tree[vertex * 2 + 1].first - tree[vertex * 2 + 1].second
                    );
        }
    }

    int get(int vertex, int left, int right, int cl, int cr) {
        if (cl > cr)
            return INT32_MAX;
        if (left == cl and right == cr)
            return tree[vertex].first - tree[vertex].second;
        else {
            int mid = (left + right) / 2;

            return std::min(
                    get(vertex * 2, left, mid, cl, std::min(cr, mid)),
                    get(vertex * 2 + 1, mid + 1, right, std::max(cl, mid + 1), cr)) - tree[vertex].second
            ;
        }
    }
};

int main() {

    int N, max = 0;
    cin >> N;

    --N;
    vector<int> temp(N, 0);
    int x;
    for (int i = 0; i < N; ++i) {
        cin >> x;

        temp[i] = -x;
    }

    cin >> max;
    for (auto& item : temp)
        item += max;

    Tree_Segments little_tree(temp);

    cin >> max;

    int begin, end, count;
    for (int i = 0; i < max; ++i) {
        cin >> begin >> end >> count;
        if (!little_tree.check_it_out(begin, end - 1, count))
            cout << i << " ";
    }

    return 0;
}
