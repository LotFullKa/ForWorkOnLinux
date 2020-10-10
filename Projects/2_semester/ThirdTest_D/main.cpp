/*
 * Шреку необходимо пробраться в башню, чтобы спасти прекрасную принцессу Фиону. И сейчас его путь лежит через старый
 * полуразвалившийся мост. Осёл очень боится упасть с этого моста в лавовую реку под ним и отказывается идти дальше,
 * пока мост не будет полностью починен. Шрек не может идти без друга и решил отремонтировать его.

    Мост представляет собой поле из n × m клеток, причем некоторые клетки ещё остались целыми.
     У Шрека есть только дощечки размера 1 × 2, установка которых занимает у него a секунд, и 1 × 1,
     установка которых занимает b секунд. Ему необходимо закрыть все пустые клетки, причем только их,
     не накладывая дощечки друг на друга.

    Определите, через какое минимальное количество секунд Шрек и Осёл смогут продолжить свой путь дальше.

    Формат ввода
    Первая строка входного файла содержит 4 целых числа n, m, a, b (1 ≤ n, m ≤ 100, |a| ≤ 1 000, |b| ≤ 1 000).
     Каждая из последующих n строк содержит по m символов: символ  «.» (точка) обозначает целую клетку моста,
     а символ «*» (звёздочка) — пустую.

    Формат вывода
    В выходной файл выведите одно число — минимальное количество секунд,
     через которое наши герои смогут продолжить свой путь, закрыв пустые клетки моста (их и только их).
 */

#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Bridge {
public:
    Bridge(const long long &price_a, const long long &price_b) : price_a(price_a), price_b(price_b) {}

    void add(const int &x , const int &y) {
        holes.emplace_back(x, y);
        if ((x + y) % 2)
            left_set.emplace_back(count);
        else
            right_set.emplace_back(count);
        count++;
    }

    long long time(){
        return shreks_calculation();
    }

private:
    vector<pair<int, int> > holes;
    vector<int> left_set;
    vector<int> right_set;
    vector< set<int> > friends;
    vector<int> matching;
    vector<int> color;
    const long long price_a;
    const long long price_b;
    int count = 0;
    int doubles = 0;

    long long shreks_calculation(){
        if (price_a >= 2 * price_b)
            return price_b * count;

        make_friends();
        color.assign(count, 0);
        matching.resize(count, -1);

        for(auto i : left_set){
            if(dfs(i)){
                ++doubles;
                color.assign(count, 0);
            }
        }

        return doubles * price_a + (count - doubles * 2) * price_b;
    }

    bool dfs(const int &ver){
        if (color[ver] != 0)
            return false;
        color[ver] = 1;

        for (auto i : friends[ver]) {
            if (matching[i] == -1 or dfs(matching[i])){
                matching[i] = ver;
                return true;
            }
        }
        return false;
    }

    void make_friends(){
        friends.resize(count);
        for (int i : left_set)
            for (int g : right_set)
                if (is_near(i, g))
                    friends[i].insert(g);
    }

    bool is_near(const int &a, const int &b){
        int delta_x = holes[a].first - holes[b].first;
        int delta_y = holes[a].second - holes[b].second;
        return 1 == (delta_x * delta_x + delta_y * delta_y);
    }
};

int main() {
    int n, m, a, b;
    cin >> n >> m >> a >> b;

    string str;
    Bridge bridge(a ,b);
    for (int i = 0; i < n; ++i) {
        cin >> str;
        for (int g = 0; g < m; ++g) {
            if (str[g] == '*')
                bridge.add(i, g);
        }
    }

    cout << bridge.time();
    return 0;
}
