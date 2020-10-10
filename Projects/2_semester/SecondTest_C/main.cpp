/*
 * Сейчас Рику надо попасть из вселенной с номером S во вселенную с номером F. Он знает все существующие телепорты,
 * и казалось бы нет никакой проблемы. Но, далеко не секрет, что за свою долгую жизнь Рик поссорился много с кем.
 * Из своего личного опыта он знает, что при телепортациях есть вероятность, что его заставят ответить за свои слова.
 *
 * Если Рик знает вероятности быть прижатым к стенке на всех существующих телепортациях,
 * помогите ему посчитать минимальную вероятность, что он всё-таки столкнется с неприятностями.
 *
 * Формат ввода
 * В первой строке даны количество вселенных N (2 ≤ N ≤ 100),
 * количество существующих телепортаций M (1 ≤ M ≤ N ⋅ (N - 1) / 2),
 * стартовая вселенная S (1 ≤ S ≤ N) и конечная вселенная F (1 ≤ F ≤ N).
 *
 * Далее в M строках дается описание телепортаций в формате Si, Fi, Pi, где Si и Fi — номера вселенных,
 * которые связывает i-я телепортация (телепорты двунаправленные, 1 ≤ Si, Fi ≤ N), Pi — вероятность быть
 * избитым на i-й телепортации в процентах (0 ≤ Pi ≤ 100).
 *
 * Формат вывода
 * Вероятность быть избитым с точностью до 10-6.
 */

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Universe{
public:
    Universe(int N, int M, int x, int y) : finish(y - 1){
        chances.resize(N, -1);
        chances[x - 1] = 1;
    }

    void add(int x, int y, int chance){
        edges.emplace_back(x - 1, y - 1, chance);
        edges.emplace_back(y - 1, x - 1, chance);
    }

    double drive(){
        build_way();
        return 1 - chances[finish];
    }

private:
    struct edge{
        edge(int x, int y, int chance) : a(x), b(y), chance(1 - 0.01 * chance) {};

        int a;
        int b;
        double chance;
    };

    vector<double> chances;
    vector<edge> edges;

    int finish;

    void build_way(){
        bool something_happen = true;
        while(something_happen){
            something_happen = false;
            for (auto & edge : edges) {
                if (chances[edge.a] != -1 and (chances[edge.b] < chances[edge.a] * edge.chance or chances[edge.b] == -1)) {
                    chances[edge.b] = chances[edge.a] * edge.chance;
                    something_happen = true;
                }
            }
        }
    }
};

int main() {
    int N, M, S, F;
    cin >> N >> M >> S >> F;

    Universe star_car(N, M, S, F);

    int x, y, chance;
    for (int i = 0; i < M; ++i) {
        cin >> x >> y >> chance;
        star_car.add(x, y, chance);
    }

    cout << round(star_car.drive() * 1000000) / 1000000 ;
    return 0;
}
