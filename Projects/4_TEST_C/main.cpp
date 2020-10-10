/*
 * У художника-авангардиста есть полоска разноцветного холста. За один раз он перекрашивает некоторый отрезок полоски
 * в некоторый цвет. После каждого перекрашивания специально обученный фотограф делает снимок
 * части получившегося творения для музея современного искусства. Для правильного экспонирования
 * требуется знать яркость самого темного цвета на выбранном фотографом отрезке. Напишите программу для
 * определения яркости самого темного цвета на отрезке.

Требуемая скорость определения яркости — O(log N).
Цвет задается тремя числами R, G и B (0 ≤ R, G, B ≤ 255), его яркость = R + G + B.
Цвет (R1, G1, B1) темнее цвета (R2, G2, B2), если R1 + G1 + B1 < R2 + G2 + B2.

Формат ввода
Первая строка содержит число N –– длина отрезка (координаты [0...N-1]).
Затем следует N строк по 3 числа –– цвета отрезка.
Следующая строка содержит число K –– количество перекрашиваний и фотографирований.
Затем задано K строк, описывающих перекрашивание и фотографирование.
В каждой такой строке 7 натуральных чисел: C, D, R, G, B, E, F, где [C, D] –– координаты перекрашиваемого отрезка,
 (R, G, B) –– цвет покраски, [E, F] –– фотографируемый отрезок.

Формат вывода
Требуется вывести K целых чисел. Каждое число –– яркость самого темного цвета на отрезке [E, F]
 после соответствующего окрашивания.


 */

#include <iostream>
#include <vector>

using namespace std;

class RMQ {
public:
    explicit RMQ(vector<int> &base) : size(base.size()) {
        tree.assign(4 * size, make_pair(INT32_MAX, -1));

        build(base, 0,size - 1, 1);
    };

    int check_it_out(int left, int right, int cl, int cr, int num) {

        update(1, 0, size - 1, left, right, num);

        return get(1, 0, size - 1, cl, cr);
    }

private:
    vector <pair<int, int>> tree;
    int size;

    void build(vector <int> &base, int left, int right, int ver){
        if (left == right){
            tree[ver].first = base[left];
        } else {
            int mid = (left + right) / 2;

            build(base, left, mid, ver * 2);
            build(base, mid + 1, right, ver * 2 + 1);

            tree[ver].first = std::min(tree[ver * 2].first, tree[ver * 2 + 1].first);
        }
    }

    void push(int ver) {
        if ((ver < tree.size()) and (tree[ver].second) != -1) {
            if (ver * 2 < tree.size()) {
                tree[ver * 2].second = tree[ver].second;
            }
            if (ver * 2 + 1 < tree.size()) {
                tree[ver * 2 + 1].second = tree[ver].second;
            }

            tree[ver].first = tree[ver].second;
            tree[ver].second = -1;
        }
    }

    void update(int ver, int left, int right, int cl, int cr, int color) {
        if (cl > cr)
            return;

        if (cl == left and cr == right)
            tree[ver].second = color;
        else {
            push(ver);

            int mid = (left + right) / 2;

            update(ver * 2, left, mid, cl, std::min(right, mid), color);
            update(ver * 2 + 1, mid + 1, right, std::max(cl, mid + 1), cr,color);

            push(ver * 2);
            push(ver * 2 + 1);

            tree[ver].first = std::min(tree[ver * 2].first, tree[ver * 2 + 1].first);
        }
    }

    int get(int ver, int left, int right, int cl, int cr) {
        if (cl > cr)
            return INT32_MAX;

        push(ver);

        if (left == cl and right == cr){
            return tree[ver].first;
        } else {
            int mid = (left + right) / 2;

            return std::min(get(ver * 2, left, mid, cl, std::min(mid, right)),
                            get(ver * 2 + 1, mid + 1, right, std::max(left, mid + 1), right)
                            );
        }
    }
};

int main() {
    int N = 0;
    cin >> N;

    vector<int> colors(N, 0);

    int R, G, B;
    for (int i = 0; i < N; ++i) {
        cin >> R >> G >> B;

        colors[i] = R + G + B;
    }

    RMQ Lines(colors);

    int k = 0;
    cin >> k;

    int C, D, E, F;
    cout << endl;
    for (int i = 0; i < k; ++i) {
        cin >>  C >> D >> R >> G >> B >> E >> F;

        cout << Lines.check_it_out(C, D, E, F, R + G + B) << " ";
    }
    return 0;
}
