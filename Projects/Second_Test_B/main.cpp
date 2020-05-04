/*
 * Рику необходимо попасть на межвселенную конференцию. За каждую телепортацию он платит бутылками лимонада,
 * поэтому хочет потратить их на дорогу как можно меньше (он же всё-таки на конференцию едет!).
 * Однако после K перелетов подряд Рика начинает сильно тошнить, и он ложится спать на день.
 * Ему известны все существующие телепортации. Теперь Рик хочет найти путь
 * (наименьший по стоимости в бутылках лимонада), учитывая, что телепортация не занимает времени,
 * а до конференции осталось 10 минут (то есть он может совершить не более K перелетов)!

Формат ввода
В первой строке находятся количество вселенных N, количество существующих телепортаций M,
 количество перелетов подряд K, номер вселенной Рика S,
 номер вселенной конференции F (2 ≤ N ≤ 300, 1 ≤ M ≤ 105, 1 ≤ K ≤ 300, 1 ≤ S ≤ N, 1 ≤ F ≤ N).

Далее идет M строк, задающих телепортации.
i-я строка содержит три натуральных числа: Si, Fi и Pi, где Si - номер вселенной, из которой ведет i-я телепортация,
Fi - номер вселенной, в которую ведет i-я телепортация, Pi - стоимость i-й телепортации. 1 ≤ Si ≤ N, 1 ≤ Fi ≤ N, 1 ≤ Pi ≤ 106.

Формат вывода
Выведите одно число - минимальную стоимость пути, подходящего для профессора.
 Если профессор не сможет за K ночей добраться до конференции, выведите число -1.
 */

#include <iostream>
#include <vector>

class Universe{
public:
    Universe(int N, int K, int x, int y) : finish(y - 1), limit(K){
        depth.resize(N, -1);
        steps.resize(N, 0);
        depth[x - 1] = 0;
    }

    void add(int x, int y, int cost){
        edges.emplace_back(x - 1, y - 1, cost);
    }

    int drive(){
        build_ways();
        return depth[finish];
    }

private:
    struct edge{

        edge(int x, int y, int cost) : a(x), b(y), cost(cost){};\

        int a;
        int b;
        int cost;
    };

    void build_ways(){
        bool something_happen = true;
        while(something_happen){
            something_happen = false;
            for (auto & edge : edges) {
                if(depth[edge.a] != -1 and steps[edge.a] < limit) {
                    if (depth[edge.a] + edge.cost < depth[edge.b] or depth[edge.b] == -1){
                        depth[edge.b] = depth[edge.a] + edge.cost;
                        steps[edge.b] = steps[edge.a] + 1;
                        something_happen = true;
                    }
                }
            }
        }
    }

    std::vector<int> depth;
    std::vector<int> steps;
    std::vector<edge> edges;

    int finish;
    int limit;

};

int main() {
    int N, M, K, x, y;
    std::cin >> N >> M >> K >> x >> y;
    Universe star_car(N, K, x, y);

    int cost;
    for (int i = 0; i < M; ++i) {
        std::cin >> x >> y >> cost;
        star_car.add(x, y, cost);
    }

    std::cout << star_car.drive();
    return 0;
}
