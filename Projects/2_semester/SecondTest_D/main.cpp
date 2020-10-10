/*
 * Рик отправляет Морти в путешествие по N вселенным. У него есть список всех существующих однонаправленных телепортов.
 * Чтобы Морти не потерялся, Рику необходимо узнать, между какими вселенными существуют пути, а между какими нет.
 * Помогите ему в этом!
 *
 * Формат ввода
 * На первой строке задано количество вселенных N (1 ≤ N ≤ 1 000). Следующие N строк имеют длину N,
 * состоят из нулей и единиц и задают матрицу смежности вселенных.
 * Единица в i-й строке j-м столбце обозначает телепорт из i-й вселенной в j-ю.
 *
 * Формат вывода
 * Выведите матрицу, где единица в i-й строке j-м столбце обозначает
 * существование пути из i-й вселенной в j-ю и ноль — его отсутствие.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>

using namespace std;

//Dao -  кит. букв. путь, в конфунцианстве толукется как "путь человека"
class Dao{
public:

    explicit Dao(int N) : size(N){}

    void add_mask(string mask){
        int num = 0;

        for (int i = 0; i < mask.size(); ++i) {
            if (mask[i] == '1')
                num += pow(2, i);
        }

        masks.push_back(num);
    }

    void print_full_map(){

        floid();

        for (auto mask : masks){
            for (int i = 0; i < size; ++i) {
                if ((mask >> i) & 1)
                    cout << 1;
                else
                    cout << 0;
            }
            cout << endl;
        }
    }

private:
    vector<int> masks;

    int size;

    void floid (){
        for (int k = 0; k < size; ++k) {
            for (int i = 0; i < size; ++i) {
                if (mask_check(i, k)){
                    for (int g = 0; g < size; ++g) {
                        if (!mask_check(i, g) and mask_check(i, k) and mask_check(k, g))
                                masks[i] += pow(2, g);
                    }
                }
            }
        }
    }

    bool mask_check(int from, int to) {
        return ((masks[from] >> to) & 1);
    }
};

int main() {
    /*
    int N;
    cin >> N;

    Dao Map(N);

    string num;
    for (int i = 0; i < N; ++i) {
        cin >> num;
        Map.add_mask(num);
    }

    Map.print_full_map();
    */
}
