/*
 * Рик решил на день почувствовать себя бизнесменом!

В городе есть несколько обменников валюты. В рамках данной задачи считаем, что каждый обменник специализируется только на двух валютах и может производить операции только с ними. Возможно, существуют обменники, специализирующиеся на одинаковых парах валют. В каждом обменнике — свой обменный курс: курс обмена A на B — это количество единиц валюты B, выдаваемое за 1 единицу валюты A. Также в каждом обменнике есть комиссия — сумма, которую вы должны заплатить, чтобы производить операцию. Комиссия взимается в той валюте, которую меняет клиент.

Например, если вы хотите поменять 100 долларов США на русские рубли в обменнике, где курс обмена равен 29.75, а комиссия равна 0.39, вы получите (100 - 0.39) ⋅ 29.75 = 2963.3975 рублей (эх, были времена).

Вы точно знаете, что в городе используется всего N валют. Пронумеруем их числами 1, 2, …, N. Тогда каждый обменник представляют 6 чисел: целые A и B — номера обмениваемых валют, а также вещественные RAB, CAB, RBA и CBA — обменные курсы и комиссии при переводе из A в B и из B в A соответственно.

Рик обладает некоторой суммой в валюте S. Он задаётся вопросом, можно ли, после нескольких операций обмена увеличить свой капитал. Конечно, он хочет, чтобы в конце его деньги вновь были в валюте S. Помогите ему ответить на его вопрос. Рик должен всегда должен иметь неотрицательную сумму денег.

Формат ввода
Первая строка содержит четыре числа: N — количество валют, M — количество обменников, S — валюта, которой располагает Рик, и V — количество единиц этой валюты. Следующие M строк содержат по 6 чисел, которые задают описание соответствующих обменников в описанном выше формате. Числа разделяются одним или несколькими пробелами. Выполняются ограничения , V — вещественное число, .

В каждом обменнике курсы обмена и комиссии — вещественные числа, задаваемые с точностью не выше двух знаков после запятой в десятичном представлении, причём .

Назовём последовательность операций обмена простой, если каждый обменник используется в ней не более одного раза. Вы можете считать, что отношение итоговой суммы Рика к исходной не будет превосходить 104 при любой простой последовательности операций.

Формат вывода
Если Рик может увеличить свой капитал, выведите YES, иначе выведите NO.

 */
#include <iostream>
#include <vector>
#include <string>

class Banks {
public:
    Banks(int N, int M, int S, double V) {
        profit.resize(N, 0);
        profit[S] = V;
        start_capital = V;
        start_val = S;
        n = N;
    }

    void add(int x, int y, double rate, double comission){
        transfers.emplace_back(x, y, rate, comission);
    }

    std::string success() {
        business();
        bool have_cycle = cycle();
        if (profit[start_val] > start_capital or have_cycle)
            return "YES";
        else if (profit[start_val] == start_capital)
            return "NO";
        else
            return "ERROR  " + std::to_string(profit[start_val]);
    }

private:
    struct edge{

        edge(int x, int y, double rate, double commission) : a(x), b(y), rate(rate), commission(commission) {}

        int a;
        int b;
        double rate;
        double commission;

        double get(double sum){
            return (sum - commission) * rate;
        }
    };

    std::vector<double> profit;
    std::vector<edge> transfers;
    double start_capital, start_val;
    int n;

    void business(){
        for (int i = 0; i < n - 1; ++i) {
            for (auto transfer : transfers) {
                if (profit[transfer.a] != 0){
                    profit[transfer.b] = std::max(profit[transfer.b], transfer.get(profit[transfer.a]));
                }
            }
        }
    }

    bool cycle() {
        for (auto transfer : transfers) {
            if (profit[transfer.a] != 0){
                if (transfer.get(profit[transfer.a]) > profit[transfer.b])
                    return true;
            }
        }
        return false;
    }

};

int main() {
    int N, M, S;
    double V;

    std::cin >> N >> M >> S >> V;

    Banks cash_machines(N, M, S - 1, V);

    int x, y;
    double r1, c1, r2, c2;

    for (int i = 0; i < M; ++i) {
        std::cin >> x >> y >> r1 >> c1 >> r2 >> c2;
        cash_machines.add(x - 1, y - 1, r1, c1);
        cash_machines.add(y - 1, x - 1, r2, c2);
    }

    std::cout << cash_machines.success();

    return 0;
}
