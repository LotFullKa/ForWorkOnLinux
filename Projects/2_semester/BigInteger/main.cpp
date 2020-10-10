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
            _bits.push_back(temp % BASE);
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
            _bits.push_back(temp % BASE);
            temp /= BASE;
        }
        return *this;
    }

    BigInteger& operator =(const BigInteger& BI){
        _sign = BI._sign;
        _bits = BI._bits;
        return *this;
    }

    // ++x;
    BigInteger& operator ++(){
        if (_sign == 0){
            _bits.resize(1, 1);
            _sign = 1;
            return *this;
        }

        if(_sign < 0){
            _sign = 1;
            --*this;
            _sign = -1;
            return *this;
        }

        unsigned int temp = 1;
        for (int i = 0; temp ; ++i) {
            if (i == _bits.size()) _bits.push_back(0);
            _bits[i] += temp;
            temp = _bits[i] / BASE;
            _bits[i] %= BASE;
        }
        return *this;
    }

    // x++;
    BigInteger operator ++(int){
        BigInteger temp = *this;
        ++*this;
        return temp;
    }

    // --x;
    BigInteger& operator --(){
        if (_sign == 0){
            _bits.resize(1, 1);
            _sign = -1;
            return *this;
        }

        if (_sign < 0){
            _sign = 1;
            ++*this;
            _sign = -1;
        }

        if (_sign > 0) {
            int i;
            for (i = 0; !_bits[i]; ++i) {
                _bits[i] = BASE - 1;
            }
            _bits[i] -= 1;
        }
        return *this;
    }

    // x--;
    BigInteger operator --(int){
        BigInteger temp = *this;
        --*this;
        return temp;
    }

    //вывод
    friend ostream& operator << (ostream &out, BigInteger BI);

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

    bool isZero() const {
        return _sign == 0 or _bits.empty() or (_bits.size() == 1 and _bits[0] == 0);
    }

    //вернуть строкой
    string toString(){
        if (isZero()) return "0";

        string for_out;

        normalization();

        for (int i = 0; i < _bits.size() - 1; ++i){
            for_out.insert(0, to_string(_bits[i]));
            for (int g = 0; g < 9 - to_string(_bits[i]).size(); ++g)
                for_out.insert(0, "0");
        }
        for_out.insert(0, to_string(_bits[_bits.size() - 1]));
        if (_sign == -1) for_out.insert(0,"-");

        while (for_out[0] == '0' and for_out.size() > 1)
            for_out.erase(0, 1);

        return for_out;
    }

    void normalization(){
        auto len = _bits.size();
        while(_bits[len - 1] == 0 and len > 1)
            --len;
        _bits.resize(len);
    }

    //бинарные операции
    friend BigInteger operator +(BigInteger &left, BigInteger &right);

    friend BigInteger operator -(BigInteger &left, BigInteger &right);


    int _sign = 1;
    vector<unsigned int> _bits;
    const int BASE = 1000000000; //10^9
};

//Оператор вывода в поток
ostream& operator<<(ostream &out, BigInteger BI) {
    cout << BI.toString();
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

    auto len = str_number.size();
    for (int i = (len + 8)/ 9 - 1; i >= 0 ; i--){
        unsigned int startP = 0;
        int k = str_number.size();
        if ((k - 9) >= 0)
            startP = str_number.size() - 9;
        BI._bits.push_back(stoi(str_number.substr(startP, 9)));
        str_number.erase(startP, 9);
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
        for (unsigned int i = left. _bits.size() - 1; i >= 0; --i){
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
BigInteger operator +(BigInteger &left, BigInteger &right){
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

        return result;
        /*
        //костыль обыкновенный - недопустимый
        left._bits = result._bits;
        return left;
         */
    }else
        return (left._sign > 0) ? (left - (-right)) : (right - (-left));
}

//вычетание
BigInteger operator -(BigInteger &left, BigInteger &right){
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

        result._bits.resize(left._bits.size());
        for (int i = 0; i < left._bits.size() - 1; ++i){
            if (left._bits[i] < right._bits[i]){
                int g;
                for (g = 1; !left._bits[i + g]; ++g){
                    left._bits[i + g] = left.BASE - 1;
                }
                --left._bits[i + g];
                left._bits[i] += left.BASE;
            }
            result._bits[i] = left._bits[i] - (i < left._bits.size() ? right._bits[i] : 0);
        }

        result._bits[left._bits.size() - 1] = left._bits[left._bits.size() - 1] - right._bits[left._bits.size() - 1];

        return result;
        /*
        //костыль обыкновенный - недопустимый
        left._bits = result._bits;
        return left;
         */

    } else if (left._sign < 0) return -((-left) + right);
    else if (right._sign < 0) return (left + (-right));
}


//          Блок умножения

void extend_vec(vector<unsigned int>& vec1, vector<unsigned int>& vec2, size_t len);

vector<unsigned int> karatsuba_mul(const vector<unsigned int>& X, const vector<unsigned int>& Y);

void normalization(vector<unsigned int> &vec);

BigInteger operator *(BigInteger &left, BigInteger &right){
        if (left.isZero() or right.isZero()) return BigInteger(0);
    BigInteger result;

    auto n = max(left._bits.size(), right._bits.size());
    extend_vec(left._bits, right._bits, n);

    if (left._sign != right._sign)
        result._sign = -1;

    result._bits = karatsuba_mul(left._bits, right._bits);
    return result;
}

vector<unsigned int> naive_mul(const vector<unsigned int> &X, const vector<unsigned int> &Y);

vector<unsigned int> karatsuba_mul(const vector<unsigned int>& X, const vector<unsigned int>& Y){
    auto len = X.size();
    vector<unsigned int> res(2 * len);

    if(len <= 1){
        return naive_mul(X, Y);
    }

    vector<unsigned int> Xl {X.begin(), X.begin() + len/2};
    vector<unsigned int> Xr {X.begin() + len/2, X.end()};
    vector<unsigned int> Yl {Y.begin(), Y.begin() + len/2};
    vector<unsigned int> Yr {Y.begin() + len/2, Y.end()};

    vector<unsigned int> P1 = karatsuba_mul(Xr, Yr);
    vector<unsigned int> P2 = karatsuba_mul(Xl, Yl);

    vector<unsigned int> Xlr(len/2);
    vector<unsigned int> Ylr(len/2);

    for (auto i = 0; i < len/2; ++i) {
        Xlr[i] = Xl[i] + Xr[i];
        Ylr[i] = Yl[i] + Yr[i];
    }

    vector<unsigned int> P3 = karatsuba_mul(Xlr, Ylr);

    for (auto i = 0; i < len; ++i) {
        P3[i] -= P2[i] + P1[i];
    }

    for (auto i = 0; i < len; ++i) {
        res[i] = P2[i];
    }

    for (auto i = len; i < 2 * len; ++i) {
        res[i] = P1[i - len];
    }

    for (auto i = len/2; i <len + len/2; ++i){
        res[i] += P3[i - len/2];
    }

    for (auto i = 0; i < res.size(); ++i) {
        res[i + 1] += res[i] / 1000000000;
        res[i] %= 1000000000;
    }

    return res;
}

vector<unsigned int> naive_mul(const vector<unsigned int> &X, const vector<unsigned int> &Y){
    auto  len = X.size();
    vector<unsigned int> res(2 * len);

    for (auto i = 0; i < len; ++i) {
        for (auto j = 0; j < len; ++j) {
            res[i + j] += X[i] * Y[j];
        }
    }

    for (auto i = 0; i < res.size(); ++i) {
        res[i + 1] += res[i] / 1000000000;
        res[i] %= 1000000000;
    }

    return res;

}

void extend_vec(vector<unsigned int>& vec1, vector<unsigned int>& vec2, size_t len){
    while (len & (len - 1)){
        ++len;
    }
    vec1.resize(len);
    vec2.resize(len);
}

void normalization(vector<unsigned int> &vec){
    auto len = vec.size();
    while(vec[vec.size() - 1] == 0 and len > 1)
        --len;
    vec.resize(len);
}

int main() {

    BigInteger a, b;
    cin >> a >> b;

    cout << a * b;

    return 0;
}

