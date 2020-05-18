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

class Tree_Segments {
public:
    Tree_Segments(const int& N, const int& my_max, vector<int>& temp) :max(my_max) {
        h = ceil(log2(N));
        ST.resize(h + 1);

        for (int i = 0; i < N; ++i) {
            ST[0].emplace_back(i, i, temp[i]);
        }

        int n = pow(2, h);
        for (int i = N; i < n; ++i) {
            ST[0].emplace_back(i, i, 0);
        }

        build();
    }

    bool push(const int& begin, const int& end, const int& count){
        if (update(h, 0, begin, end - 1, count))
            return true;
        else {
            update(h, 0, begin, end - 1, -count);
            return false;
        }
    }

private:
    struct segment{

        segment(const int& l, const int& r, const int& max) : left(l), right(r), max(max) {}

        int left;
        int right;
        int add = 0;
        int max;

        int mid() const {
            return (left + right + 1)/ 2;
        }
    };

    vector < vector<segment> > ST;
    int max;
    int h;

    void build(){
        for (int k = 1; k < h + 1; ++k) {
            for (int i = 0; i < ST[k - 1].size() / 2; ++i) {
                segment result(
                        ST[k - 1][2 * i].left,
                        ST[k - 1][2 * i + 1].right,
                        std::max(ST[k - 1][2 * i].max, ST[k - 1][2 * i + 1].max)
                        );
                ST[k].emplace_back(result);
            }
        }
    }

    bool update(const int& k, const int& n, const int& begin, const int& end, int count) {
        segment& me = ST[k][n];
        if (me.left == begin and me.right == end) {
            me.add += count;
            me.max += count;
            return me.max <= max;
        }
        ST[k - 1][2 * n].max += me.add;
        ST[k - 1][2 * n + 1].max += me.add;
        me.add = 0;
        bool flag;
        if (begin >= me.mid()) {
            flag = update(k - 1, 2 * n + 1, begin, end, count);
            me.max = std::max(ST[k - 1][2 * n].max, ST[k - 1][2 * n + 1].max);
        } else if (end < me.mid()) {
            flag = update(k - 1, 2 * n, begin, end, count);
            me.max = std::max(ST[k - 1][2 * n].max, ST[k - 1][2 * n + 1].max);
        } else {
            flag =
            update(k - 1, 2 * n, begin, me.mid() - 1, count)
            and
            update(k - 1, 2 * n + 1, me.mid(), end, count);
            me.max = std::max(ST[k - 1][2 * n].max, ST[k - 1][2 * n + 1].max);
        }
        return flag;
    }

};

int main() {

    int N, m;
    cin >> N;

    N--;
    vector<int> temp;
    int x;
    for (int i = 0; i < N; ++i) {
        cin >> x;
        temp.emplace_back(x);
    }
    cin >> m;
    Tree_Segments my_little_tree(N, m, temp);

    cin >> m;
    int begin, end, count;
    for (int i = 0; i < m; ++i) {
        cin >> begin >> end >> count;
        if (!my_little_tree.push(begin, end, count))
            cout << i << " ";
    }


    return 0;
}
