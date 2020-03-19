#include <iostream>
#include <vector>
#include <string>

using namespace std;

class BigInteger {
public:
    BigInteger() = default;

    friend ostream& operator << (ostream &out, BigInteger &BI);

    friend istream& operator >> (istream &in, BigInteger &BI);

    friend BigInteger operator +(const BigInteger &BI);

    friend BigInteger operator -(BigInteger &BI);

    friend bool operator ==(BigInteger &left, BigInteger &right);

    friend bool operator <(BigInteger &left, BigInteger &right);

    friend BigInteger operator +(BigInteger &BI, int x);

    friend BigInteger operator +(int x, BigInteger &BI);

private:
    int _sign = 1;
    vector<long int> _bits;
    const int BASE = 1000000; //10^6

};

//Оператор вывода в поток
ostream &operator<<(ostream &out, BigInteger &BI) {
    if (BI._sign == -1) cout << "-";
    for(auto i : BI._bits)
        cout << i;
    return out;
}

//Опретар ввода из поток
istream &operator>>(istream &in, BigInteger &BI) {
    string str_number;
    getline(in, str_number);

    if(str_number[0] == '-') {
        str_number.erase(0,1);
        BI._sign = -1;
    }
    if(str_number[0] == '+'){
        str_number.erase(0,1);
        BI._sign = 1;
    }

    if(str_number == "0") {
        BI._sign = 0;
        return in;
    }

    while (str_number[0] == '0')
        str_number.erase(0, 1);

    for (int i = 0; i < (str_number.size() + 5)/ 6; ++i) {
        BI._bits.push_back(stoi(str_number.substr(i*6, 6)));
    }

    return in;
}

//унарный + (тупо плюсик перед числом а ля y = +x;)
BigInteger operator+(const BigInteger &BI) {
    return BI;
}

//унарный минус перед числом (y = -x )
BigInteger operator-(BigInteger &BI) {
    BI._sign *= -1;
    return BI;
}

//                          Блок сравнений
//сравнения равенства

bool operator ==(BigInteger &left, BigInteger &right) {
    //сравнение по знаку
    if (left._sign != right._sign) return false;

    //случай когда оба числа равны нулю
    if (left._sign == 0 and right._sign == 0) return true;

    //проверка длинны чисел
    if (left._bits.size() != right._bits.size()) return false;

    //только теперь начнем сравнивать их int компоненты
    for (size_t i = 0; i < left._bits.size(); ++i)
        if (left._bits[i] != right._bits[i]) return false;

    //выживший герой ! ты добрался ! ! !
    return true;
}

//сравнение строго неравенства
bool operator <(BigInteger &left, BigInteger &right) {
    if (left == right) return false;

    if (left._sign < 0){
        if (right._sign > 0) return true;
        else return ((right) < (left));
    } else if (right._sign < 0) return false;
    else if (left._bits.size() != right._bits.size()) {
            return left._bits.size() < right._bits.size();
        } else for (size_t i = 0; i < left._bits.size(); ++i) {
            if (left._bits[i] != right._bits[i]) return left._bits[i] < right._bits[i];
            return false;
        }
}


int main() {
    BigInteger kak, tak;

    cin >> kak;
    cout << -kak;

    return 0;
}

