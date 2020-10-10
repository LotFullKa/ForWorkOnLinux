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
#include <bitset>

using namespace std;

//Dao -  кит. букв. путь, в конфунцианстве толукется как "путь человека"
class Dao{
public:

    explicit Dao(int N) : size(N){
        masks.resize(size);
        for (int i = size - 1; i > -1; --i){
            cin >> masks[i];
        }
    }

    void print_full_map(){
        floid();
        for (int i = size - 1; i > -1; --i) {
            cout << masks[i].to_string().substr(Base - size, size) << endl;
        }
    }


private:
    static const int Base = 1000;
    int size;
    vector<bitset<Base>> masks;

    void floid() {
        for (int k = 0; k < size; ++k) {
            for (int i = 0; i < size; ++i) {
                if (masks[i][k]){
                    masks[i] |= masks[k];
                }
            }
        }
    }

};

int main() {

    int N;
    cin >> N;

    Dao Map(N);

    Map.print_full_map();
}
