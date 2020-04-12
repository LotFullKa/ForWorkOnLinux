/*
 * Рик и Морти снова бороздят просторы вселенных, но решили ограничиться только теми, номера которых меньше M.
 * Они могут телепортироваться из вселенной с номером z во вселенную (z+1) mod M за a бутылок лимонада или во вселенную
 * (z2+1) mod M за b бутылок лимонада. Рик и Морти хотят добраться из вселенной с номером x во вселенную с номером y.
 * Сколько бутылок лимонада отдаст Рик за такое путешествие, если он хочет потратить их как можно меньше?
 *
 * Формат ввода
 * В строке подряд даны количество бутылок a за первый тип телепортации, количество бутылок b за второй тип
 * телепортации, количество вселенных M, номер стартовой вселенной x,
 * номер конечной вселенной y (0 ≤ a, b ≤ 100, 1 ≤ M ≤ 106, 0 ≤ x, y < M).
 */

#include <iostream>
#include <vector>
#include <deque>
#include <queue>

using namespace std;

class Graph{
public:
    Graph(int a, int b, int M, int x, int y) : a(a), b(b), M(M), x(x), y(y){
        depth.resize(M, -1);
        depth[x] = 0;
    }

    pair<int, int> find_cost(){
        que.push(make_pair(0, x));
        while (que.top().second != y){
            int ver = que.top().second;
            que.pop();
            DBFS(ver);
        }
        return make_pair(depth[y], que.top().first);
    }

private:
    vector<int> depth;
    priority_queue <pair<int, int>,  vector <pair<int, int>>, greater< >> que;
    int x, y, a, b, M;

    long int first_jump(long int z){
        return (z + 1) % M;
    }

    long int second_jump(long int z){
        return (z*z + 1) % M;
    }

    void DBFS(int vertex){
        long int first_point = first_jump(vertex);
        long int second_point = second_jump(vertex);

        if (depth[vertex] + a < depth[first_point] or depth[first_point] == -1) {
            int weight = depth[vertex] + a;

            que.push(make_pair(weight, first_point));
            depth[first_point] = weight;
        }

        if (depth[vertex] + b < depth[second_point] or depth[second_point] == -1) {
            int weight = depth[vertex] + b;

            que.push(make_pair(weight, second_point));

            depth[second_point] = weight;
        }
    }
};


int main() {
    int a, b, M, x, y;

    cin >> a >> b >> M >> x >> y;
    Graph star_car(a, b, M, x, y);

    cout << star_car.find_cost().second << endl;
    return 0;
}
