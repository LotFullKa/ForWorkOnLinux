/*
 * Дано число N и последовательность из N целых чисел. Найти вторую порядковую статистику на заданных диапазонах.

Для решения задачи используйте структуру данных Sparse Table. Требуемое время обработки каждого диапазона O(1).
Время подготовки структуры данных .

Формат ввода
В первой строке заданы 2 числа: размер последовательности N и количество диапазонов M.
Следующие N целых чисел задают последовательность. Далее вводятся M пар чисел - границ диапазонов.
Гарантируется, что каждый диапазон содержит как минимум 2 элемента.
Формат вывода
Для каждого из M диапазонов напечатать элемент последовательности - 2ю порядковую статистику. По одному числу в строке.
 */

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class Range {
public:
    Range (int& N): len(N) {
        pre_calculation_log();
        ST.resize(logs[len - 1]+1);
    }

    void add_array(vector <int> & array) {
        for (int i = 0; i < array.size(); ++i) {
            ST[0].emplace_back(i, array[i], array[i]);
        }
        pre_calculation();
    }

    int get_second_statistic(const int& l, const int& r) {
        int k = logs[r - l];
        return boxing( k, l - 1, r - pow(2, k)).sec_min;
    }

private:

    struct Box {

        Box(const int& id,const int& min,const int& sec_min) : id(id), min(min), sec_min(sec_min) {}

        int id;
        int min;
        int sec_min;
    };

    vector < vector<Box> > ST;
    vector <int> logs;
    int len;

    void pre_calculation_log(){
        for (int i = 0; i < len; ++i) {
            logs.emplace_back(int(log2(i + 1)));
        }
    }

    void pre_calculation() {
        for (int k = 1; k < logs[len - 1] + 1; ++k) {
            for (int i = 0; i < len + 1 - pow(2, k); ++i) {
                ST[k].push_back(boxing(k - 1, i, i + pow(2, k - 1)));
            }
        }
    }

    Box boxing(const int& k,const int& a,const int& b){
        if (ST[k][a].id == ST[k][b].id) {
            Box result(ST[k][a].id, ST[k][a].min, min(ST[k][a].sec_min, ST[k][b].sec_min));
            return result;
        } else if (ST[k][a].min < ST[k][b].min) {
            if (ST[k][a].sec_min < ST[k][b].min and k != 0)
                return ST[k][a];
            else {
                Box result(ST[k][a].id, ST[k][a].min, ST[k][b].min);
                return result;
            }
        } else {
            if (ST[k][b].sec_min < ST[k][a].min and k != 0)
                return ST[k][b];
            else {
                Box result(ST[k][b].id, ST[k][b].min, ST[k][a].min);
                return result;
            }
        }
    }
};

int main() {
    
    int n, m;
    cin >> n >> m;
    Range range(n);
    
    vector<int> temp;
    int x, y;
    for (int i = 0; i < n; ++i) {
        cin >> x;
        temp.emplace_back(x);
    }
    
    range.add_array(temp);
    
    for (int i = 0; i < m; ++i) {
        cin >> x >> y;
        cout << range.get_second_statistic(x, y) << endl;
    }
    return 0;
}
