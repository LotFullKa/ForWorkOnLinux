#include <iostream>
#include <vector>
#include <string>

using namespace std;

class BigInteger {
public:
    //default
    BigInteger()= default;

    //by int
    BigInteger(const int x){
        int temp  = x;
        if (temp == 0) _sign = 0;
        else if (temp < 0){
            _sign = -1;
            temp *= -1;
        }
        while (temp != 0){
            _bits.push_back(x % BASE);
            temp /= BASE;
        }
    }

    // copy
    BigInteger(BigInteger const &BI){
        _sign = BI._sign;
        _bits = BI._bits;
    }

    // to bool
    operator bool(){
        return _sign != 0 && true;
    }

    BigInteger& operator =(const int& x){
        int temp  = x;
        if (temp == 0) _sign = 0;
        else if (temp < 0){
            _sign = -1;
            temp *= -1;
        }
        while (temp != 0){
            _bits.push_back(x % BASE);
            temp /= BASE;
        }
        return *this;
    }

    BigInteger& operator =(const BigInteger& BI){
        _sign = BI._sign;
        _bits = BI._bits;
    }

    //вывод
    friend ostream& operator << (ostream &out, BigInteger &BI);

    //ввод
    friend istream& operator >> (istream &in, BigInteger &BI);

    //унарный плюс
    BigInteger& operator+ () {
        return *this;
    }

    //унарный минус
    BigInteger& operator- () {
        this->_sign *= -1;
        return *this;
    }

    //                  Сравнения

    //сравнение равенства
    friend bool operator ==(const BigInteger &left, const BigInteger &right);

    //сравнения неравенства
    friend bool operator !=(const BigInteger &left, const BigInteger &right);

    //сравнение меньше
    friend bool operator <(const BigInteger &left, const BigInteger &right);

    //сравнение меньше или равно
    friend bool operator <=(const BigInteger &left, const BigInteger &right);

    //сравнение больше
    friend bool operator >(const BigInteger &left, const BigInteger &right);

    //сравнение больше или равно
    friend bool operator >=(const BigInteger &left, const BigInteger &right);


    //вернуть строкой
    string toString() const {
        if (_sign == 0 or _bits.empty()) return "0";

        string for_out;

        for(auto i : _bits)
            for_out.insert(0, to_string(i));

        if (_sign == -1) for_out.insert(0,"-");

        return for_out;
    }

    //бинарные операции
    friend BigInteger& operator +(BigInteger &left, BigInteger &right);

    friend BigInteger& operator -(BigInteger &left, BigInteger &right);


    int _sign = 1;
    vector<unsigned int> _bits;
    const int BASE = 1000000000; //10^9
};

//Оператор вывода в поток
ostream& operator<<(ostream &out, BigInteger &BI) {
    if (BI._sign == 0 or BI._bits.empty()) {
        cout << "0";
        return out;
    }

    if (BI._sign == -1) cout << "-";
    for(long int i = BI._bits.size() - 1; i >= 0; --i)
        cout << BI._bits[i];
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

    while (str_number[0] == '0' and str_number.size() > 1)
        str_number.erase(0, 1);

    if(str_number == "0") {
        BI._sign = 0;
        return in;
    }

    for (int i = (str_number.size() + 8)/ 9 - 1; i >= 0 ; i--){
        BI._bits.push_back(stoi(str_number.substr(i*9, 9)));
    }

    return in;
}

//              Блок сравнений !

//сравнение равнества
bool operator ==(const BigInteger &left, const BigInteger &right){
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

//сравнение неравенства
bool operator !=(const BigInteger &left, const BigInteger &right){
    return !(left == right);
}

//сравнение меньше
bool operator <(const BigInteger &left, const BigInteger &right){

    //если знаки совпадают
    if (left._sign == right._sign){
        //если длины не равны
        if (left._bits.size() < right._bits.size())
            return (left._bits.size() < right._bits.size()) * left._sign;

        //если длины равны, ишем первый отличающийся разряд, отвечаем
        for (size_t i = 0; i <left. _bits.size(); ++i){
            if(left._bits[i] != right._bits[i])
                return  (left._bits[i] < right._bits[i]) * left._sign;
        }

        //случай с нулями
        return false;
    } else
        return left._sign;
}

//сравнение меньше или равно
bool operator <=(const BigInteger &left, const BigInteger &right){
    return (left < right or left == right);
}

//сравнение больше
bool operator >(const BigInteger &left, const BigInteger &right){
    return !(left < right or left == right);
}

//сравнение больше или равно
bool operator >=(const BigInteger &left, const BigInteger &right){
    return (left > right or left == right);
}

//сложение
BigInteger& operator +(BigInteger &left, BigInteger &right){
    if (left._sign == 0) return right;
    if (right._sign == 0) return left;

    if (left._sign == right._sign){

        BigInteger result;
        unsigned int lenght = (left._bits.size() < right._bits.size() ? right._bits.size() : left._bits.size()) + 1;

        result._bits.resize(lenght, 0);

        for (int i = 0; i < lenght - 1; ++i) {
            result._bits[i] += (i < left._bits.size() ? left._bits[i] : 0) + (i < right._bits.size() ? right._bits[i] : 0);
            result._bits[i + 1] += result._bits[i] / result.BASE;
            result._bits[i] %= result.BASE;
        }

        if (result._bits[lenght - 1] == 0)
            result._bits.resize(lenght - 1);

        //костыль обыкновенный - недопустимый
        left._bits = result._bits;
        return left;
    }else
        return (left._sign > 0) ? (left - (-right)) : (right - (-left));
}

BigInteger& operator -(BigInteger &left, BigInteger &right){
    if (left._sign == 0) return -right;
    if (right._sign == 0) return left;

    if (left == right){
        left._sign = 0;
        return left;
    }

    if (left._sign < 0 and right._sign < 0) return -((-left) - (-right));

    if (left._sign == right._sign){
        if (left < right) return -(right - left);

        BigInteger result;

        int lenght = max(left._bits.size(), right._bits.size());

        result._bits.resize(lenght);
        for (int i = 0; i < lenght - 1; ++i){
            if (left._bits[i] < right._bits[i]){
                left._bits[i + 1]--;
                left._bits[i] += left.BASE;
            }
            result._bits[i] = left._bits[i] - right._bits[i];
        }

        result._bits[lenght - 1] = left._bits[lenght - 1] - right._bits[lenght - 1];

        //костыль обыкновенный - недопустимый
        left._bits = result._bits;
        return left;

    } else if (left._sign < 0) return -((-left) + right);
    else if (right._sign < 0) return (left + (-right));
}


int main() {
    BigInteger a, b;

    cin >> a >> b;
    cout << a - b;
    return 0;
}

