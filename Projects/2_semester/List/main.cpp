#include <iostream>
#include <vector>

using namespace std;
class Base{
public:
    virtual void f() {cout << 1;}
};

class Dev: public Base {
public:
    virtual void f() {cout << 2;}
};

int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    v[5] = 7;
    cout << v[5];


    //класс отличается от структуры модификацией поля видимости
    // в случае класса оно сразу Private
    // в случае структуры оно сразу public
    //там написали стракт чтобы не писать +1 стрчоку паблик )
    // пока тебе объясняю чувствую себя дохуя умным (но нет)
    //ТАК НО ТУТ ТОЧНО НЕ ОШИБКА ПОТОМУ ЧТО Я ЗНАЮ КАК РАБОТАЮТ  ВИРТУАЛКИ )
    //втф

}
