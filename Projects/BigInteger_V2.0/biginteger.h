#include <iostream>
#include <vector>
#include <string>

using namespace std;

class BigInteger {
public:
    // конструкторы и конвентаторы

    //default
    BigInteger(){
        _digits.resize(0);
        _digits.push_back(0);
    }

    //by int
    BigInteger(int x) {
        _digits.resize(0);
        if (x == 0) {
            _digits.push_back(0);
        } else if (x > 0){
            while (x != 0) {
                _digits.push_back(x % BASE);
                x /= BASE;
            }
        } else {
            x = -x;
            _sign = false;
            while (x != 0) {
                _digits.push_back(x % BASE);
                x /= BASE;
            }
        }
    }

    //by string
    BigInteger(string str_number){
        _digits.resize(1);
        if(str_number[0] == '-'){
            _sign = false;
            str_number.erase(0,1);
        } else if(str_number[0] == '+')
            str_number.erase(0, 1);

        int digit = 0;
        int num = 1;
        for (int i = str_number.size() - 1; i >= 0; --i) {
            if (num == BigInteger::BASE) {
                ++digit;
                num = 1;
                _digits.push_back(0);
            }
            if (num < BigInteger::BASE) {
                _digits[digit] += num *(str_number[i] - '0');
                num *= 10;
            }
        }
    }

    //copy
    BigInteger(const BigInteger &BI){
        _sign = BI._sign;
        _digits = BI._digits;
    }

    explicit operator bool() const {
        return !isZero();
    }

    //методы и операторы

    //число в виде строки
    string toString() const {
        string for_out;
        string temp;
        for_out += to_string(_digits[size() - 1]);
        for (int i = size() - 2; i >= 0; --i) {
            temp = to_string(_digits[i] + BigInteger::BASE);
            for_out += temp.substr(1);
        }
        if(!_sign)
            for_out.insert(0, "-");

        return for_out;
    }

    //навиное умножение
    static BigInteger naiv_mul(const BigInteger &left, const BigInteger &right){
        BigInteger result;
        if (left.isZero() or right.isZero()) {
            result = 0;
            return result;
        }

        result._sign = !(left._sign xor right._sign);
        for (int i = 0; i < left.size(); ++i) {
            for (int j = 0; j < right.size(); ++j) {
                if(result.size() <= i + j)
                    result._digits.push_back(0);
                result._digits[i + j] += left._digits[i] * right._digits[j];
            }
        }

        long long int temp = 0;
        for (int i = 0; i < result.size(); ++i) {
            temp = result._digits[i] / BigInteger::BASE;
            if (temp != 0) {
                if (i + 1 >= result.size())
                    result._digits.push_back(0);
                result._digits[i + 1] += temp;
            }
            result._digits[i] %= BigInteger::BASE;
        }
        return result;
    }

    //деление на два с округлением в меньшую сторону
    BigInteger devision_by_two(){
        BigInteger dividend = *this;
        int modulo = 0;
        for(int i = dividend.size() - 1; i > - 1; --i){
            int temp = (dividend._digits[i] + modulo) / 2;
            if (dividend._digits[i] % 2 == 1)
                modulo = BASE;
            else
                modulo = 0;
            dividend._digits[i] = temp;
        }

        dividend.normalize();

        return dividend;
    }

    //унарный минус
    BigInteger operator -() const {
        if (isZero()) return *this;

        BigInteger temp = *this;
        temp._sign = !this->_sign;
        return temp;
    }

    //унарный плюс
    BigInteger operator +() const {
        return *this;
    }

    //префиксный инкремент ++х
    BigInteger& operator ++(){
        *this += 1;
        return *this;
    }

    //префиксный декримент --х
    BigInteger& operator --(){
        *this -= 1;
        return  *this;
    }

    //постфиксный инкремент х++
    BigInteger operator ++(int){
        BigInteger temp = *this;
        *this += 1;
        return temp;
    }

    //постфиксный декремент х--
    BigInteger operator --(int){
        BigInteger temp = *this;
        *this -= 1;
        return temp;
    }

    //самосложение
    BigInteger& operator +=(const BigInteger& right){
        *this = *this + right;
        return *this;
    }

    //самовычитание
    BigInteger& operator -=(const BigInteger& right){
        *this = *this - right;
        return  *this;
    }

    //самоумножение
    BigInteger& operator *=(const BigInteger& right){
        *this = (*this) * right;
        return *this;
    }

    //самоделение
    BigInteger& operator /=(const BigInteger& right){
        *this = *this / right;
        return *this;
    }

    BigInteger& operator %=(const BigInteger& right){
        *this = *this % right;
        return *this;
    }


    //бинарные операторы

    //сравнения
    friend bool operator < (const BigInteger &left, const BigInteger &right);

    friend bool operator < (const int &left, const BigInteger &right);

    friend bool operator < (const BigInteger &left, const int &right);

    //сложение
    friend BigInteger operator +(const BigInteger &left, const BigInteger &right);

    //вычитание
    friend BigInteger operator -(const BigInteger &left, const BigInteger &right);

    friend BigInteger operator *(const BigInteger &left, const BigInteger &right);

    friend BigInteger operator /(const BigInteger &left, const BigInteger &right);

    friend BigInteger operator %(const BigInteger &left, const BigInteger &right);

private:
    // компоненты и функции личного пользования
    static const int BASE = 10000; //10^5
    bool _sign = true;
    vector<int> _digits;

    int size() const{
        return _digits.size();
    }

    //true if x = 0
    bool isZero() const {
        return size() == 1 and _digits[0] == 0;
    }

    void normalize(){
        int len = size();
        while(len > 1 and _digits[len - 1] == 0)
            --len;
        _digits.resize(len);
    }

    int save_digit(int i) const {
        if (i < size())
            return _digits[i];
        else
            return 0;
    }
};

istream& operator >>(istream& in, BigInteger& BI){
    string in_str;
    in >> in_str;
    BI = BigInteger(in_str);
    return  in;
}

ostream& operator <<(ostream& out, const BigInteger& BI){
    out << BI.toString();
    return out;
}

//сравнение BigInt
bool operator <(const BigInteger &left, const BigInteger &right) {
    if (!left._sign and right._sign)
        return true;
    else if (left._sign and !right._sign)
        return  false;
    else if (left._sign and right._sign){
        if (left.size() < right.size())
            return true;
        else if (left.size() > right.size())
            return false;
        else {
            for (int i = left.size() - 1; i >=0; --i){
                if (left._digits[i] < right._digits[i])
                    return true;
                if (left._digits[i] > right._digits[i])
                    return false;
            }
            return false;
        }
    } else {
        return (-right < -left);
    }
}

bool operator >(const BigInteger &left, const BigInteger &right){
    return right < left;
}

bool operator <=(const BigInteger &left, const BigInteger &right){
    return !(left > right);
}

bool operator >=(const BigInteger &left, const BigInteger &right){
    return !(left < right);
}

bool operator ==(const BigInteger &left, const BigInteger &right){
    return !(left < right or right < left);
}

bool operator !=(const BigInteger &left, const BigInteger &right){
    return (left < right or right < left);
}

//сравнение BigInt и Int
bool operator <(const BigInteger &left, const int &right) {
    return (left < BigInteger(right));
}

bool operator >(const BigInteger &left, const int &right){
    return right < left;
}

bool operator <=(const BigInteger &left, const int &right){
    return !(left > right);
}

bool operator >=(const BigInteger &left, const int &right){
    return !(left < right);
}

bool operator ==(const BigInteger &left, const int &right){
    return !(left < right or right < left);
}

bool operator !=(const BigInteger &left, const int &right){
    return (left < right or right < left);
}

//сравнение Int и BigInt
bool operator <(const int &left, const BigInteger &right){
    return (BigInteger(left) < right);
}

bool operator >(const int &left, const BigInteger &right){
    return right < left;
}

bool operator <=(const int &left, const BigInteger &right){
    return !(left > right);
}

bool operator >=(const int &left, const BigInteger &right){
    return !(left < right);
}

bool operator ==(const int &left, const BigInteger &right){
    return !(left < right or right < left);
}

bool operator !=(const int &left, const BigInteger &right){
    return (left < right or right < left);
}

//сложение
BigInteger operator +(const BigInteger &left, const BigInteger &right){
    if (left.isZero()) return right;
    if (right.isZero()) return left;

    if (left._sign == right._sign){
        BigInteger result;
        result._digits.resize(max(left.size(), right.size()) + 1, 0);

        for (int i = 0; i < result.size() - 1; ++i) {
            result._digits[i] += left.save_digit(i) + right.save_digit(i);
            result._digits[i + 1] = result._digits[i] / BigInteger::BASE;
            result._digits[i] %= BigInteger::BASE;
        }

        if (result._digits[result.size() - 1] == 0)
            result._digits.resize(result.size() - 1);

        result._sign = left._sign;
        return result;
    } else if (left._sign)
        return (left - (-right));
    else
        return (right - (-left));
}

//вычитание
BigInteger operator -(const BigInteger &left, const BigInteger &right){
    if (left.isZero()) return -right;
    if (right.isZero()) return left;

    if (left._sign == right._sign and !left._sign)
        return -((-left) - (-right));
    if (left._sign == right._sign and left._sign){
        if (left >= right) {
            BigInteger result;
            result._digits.resize(left.size());
            int dept = 0;
            for (int i = 0; i < result.size(); ++i) {
                long long int val_of_left = left.save_digit(i);
                long long int val_of_right = right.save_digit(i);
                if ((val_of_left + dept) >= val_of_right) {
                    result._digits[i] = val_of_left + dept - val_of_right;
                    dept = 0;
                } else {
                    result._digits[i] = val_of_left + dept + BigInteger::BASE - val_of_right;
                    dept = -1;
                }
            }
            result._sign = left._sign;
            result.normalize();

            return result;
        } else
            return -(right - left);
    } else if (left._sign)
        return (left + (-right));
    else
        return -((-left) + right);

}

//умножение
BigInteger operator*(const BigInteger &left, const BigInteger &right) {
    BigInteger X = left;
    BigInteger Y = right;

    //проверка на ноль !
    X.normalize();
    Y.normalize();
    if (X.isZero() or Y.isZero())
        return 0;

    int len = max(left.size(), right.size());
    if(len <= 10)
        return BigInteger::naiv_mul(X, Y);

    BigInteger result;
    result._sign = !(X._sign xor Y._sign);
    int k = (len + 1)/2;

    BigInteger Xl, Xr;
    if(X.size() < k){
        Xl = X;
        Xr = 0;
    } else {
        Xl._digits.resize(k);
        Xr._digits.resize(X.size() - k);
        copy(X._digits.begin(), X._digits.begin() + k, Xl._digits.begin());
        copy(X._digits.begin() + k, X._digits. end(), Xr._digits.begin());
    }

    BigInteger Yl, Yr;
    if(Y.size() < k){
        Yl = Y;
        Yr = 0;
    } else {
        Yl._digits.resize(k);
        Yr._digits.resize(Y.size() - k);
        copy(Y._digits.begin(), Y._digits.begin() + k, Yl._digits.begin());
        copy(Y._digits.begin() + k, Y._digits.end(), Yr._digits.begin());
    }

    BigInteger Xlr = Xl + Xr;
    BigInteger Ylr = Yl + Yr;

    BigInteger P1 = Xl * Yl;
    BigInteger P2 = Xr * Yr;
    BigInteger P3 = Xlr * Ylr;
    P3 -= P1 + P2;

    result._digits.resize(len * 2, 0);
    for (int i = 0; i < len; ++i) {
        result._digits[i] = P1.save_digit(i);
    }

    for (int i = len; i < 2 * len; ++i){
        result._digits[i] = P2.save_digit(i - len);
    }

    for (int i = 0; i < len; ++i) {
        result._digits[i + k] += P3.save_digit(i);
    }

    result._digits.push_back(0);
    for (int i = 0; i < 2 * len; ++i) {
        result._digits[i + 1] += result._digits[i] / BigInteger::BASE;
        result._digits[i] %= BigInteger::BASE;
    }

    result.normalize();

    return result;
}

//деление
BigInteger operator /(const BigInteger &left, const BigInteger &right){
    if (left.isZero())
        return  0;

    BigInteger a = left;
    a._sign = true;
    BigInteger b = right;
    b._sign = true;

    if(a < b){
        return 0;
    } else if (a > b) {
        BigInteger first = 1;
        BigInteger second = a + 1;
        BigInteger result = (first + second).devision_by_two();

        while (first < second - 1){
            result = (first + second).devision_by_two();

            BigInteger temp = result * b;
            if (temp < a){
                first = result;
            } else if (temp > a){
                second = result;
            } else {
                second = result;
                break;
            }
        }

        BigInteger main_result = second;
        main_result._sign = !(left._sign xor right._sign);
        if (main_result._sign and a != second * b)
            return --main_result;

        return main_result;
    } else {
        BigInteger main_result(1);
        main_result._sign = !(left._sign xor right._sign);
        return main_result;
    }
}

//остаток от деления
BigInteger operator %(const BigInteger &left, const BigInteger &right){
    BigInteger result = left - right * (left / right);
    return result;
}


// Rational
class Rational{
private:
    static const int BASE = 1000000000;
    static const int BASE_len = 9;
    BigInteger numerator;
    BigInteger denominator;
    bool _sign = true;

    static BigInteger greatest_common_factor(const BigInteger &a,const BigInteger &b){
        if (b != 0)
            return greatest_common_factor(b, a % b);
        else
            return a;
    }

public:
    //default
    Rational(){
        numerator = 0;
        denominator = 1;
    };

    //by int
    Rational(int x){
        if (x >= 0) {
            _sign = true;
            numerator = x;
        } else {
            _sign = false;
            numerator = -x;
        }
        denominator = 1;
    }

    //by BigInteger
    Rational(const BigInteger& BI){
        if (BI >= 0){
            _sign = true;
            numerator = BI;
        } else {
            _sign = false;
            numerator = -BI;
        }
        denominator = 1;
    }

    //copy
    Rational(const Rational& Ra){
        _sign = Ra._sign;
        numerator = Ra.numerator;
        denominator = Ra.denominator;
    }

    //by string
    Rational(const string str){
        string num = "";
        string denom = "";


        unsigned int i = 0;
        if (str[0] == '-'){
            _sign = false;
            i = 1;
        }

        for (; str[i] != '/' ; ++i) {
            num += str[i];
        }

        ++i;
        for (; i < str.size(); ++i){
            denom +=  str[i];
        }

        numerator = BigInteger(num);
        denominator = BigInteger(denom);
        BigInteger temp = greatest_common_factor(numerator, denominator);
        numerator /= temp;
        denominator /= temp;
    }

    explicit operator bool(){
        return bool(numerator);
    }

    explicit operator double(){
        string temp = asDecimal(500);
        return stod(temp);
    }

    string toString() const {
        string str_out = "";
        if(!_sign)
            str_out += "-";
        str_out += numerator.toString();
        if(denominator != 1 or denominator != 0){
            str_out += "/";
            str_out += denominator.toString();
        }
        return str_out;
    }

    string asDecimal(size_t precision = 0){
        BigInteger integer = numerator / denominator;
        BigInteger  rem = numerator % denominator;

        string frac;
        string temp;
        while (frac.size() <= precision) {
            temp = ((rem * BigInteger(BASE)) / denominator).toString();
            for (size_t i = 0; i < (BASE_len - temp.size()); ++i) {
                frac += '0';
            }
            frac += temp;
            rem = (rem * BigInteger(BASE)) % denominator;
        }
        int i = precision;
        if(frac[i] - '4' > 0){
            --i;
            while (i>= 0 and frac[i] == '9') {
                frac[i] = '0';
                --i;
            }
            if(i == -1)
                ++integer;
            else
                ++frac[i];
        }
        if(precision == 0 and frac[0] - '4' > 0)
            ++integer;

        string result;
        if(!_sign)
            result += '-';

        result += integer.toString() + ".";
        for(size_t j = 0; j < precision; ++j)
            result += frac[j];

        return result;
    }

    Rational operator -() const{
        Rational result = *this;
        result._sign = ! this->_sign;
        return  result;
    }

    Rational operator +() const{
        return *this;
    }

    Rational& operator +=(const Rational &right){
        if(_sign and !right._sign){
            BigInteger temp = denominator;
            numerator *= right.denominator;
            denominator *= right.denominator;
            numerator -= right.numerator * temp;
            if (numerator < 0){
                _sign = false;
                numerator = -numerator;
            }
            temp = greatest_common_factor(numerator, denominator);
            numerator /= temp;
            denominator /= temp;
        } else if (!_sign and right._sign) {
            Rational temp = -*this += -right;
            *this = -temp;
        } else {
            BigInteger temp = denominator;
            numerator *= right.denominator;
            denominator *= right.denominator;
            numerator += right.numerator * temp;
            temp = greatest_common_factor(numerator, denominator);
            numerator /= temp;
            denominator /= temp;
        }

        return *this;
    }

    Rational& operator -=(const Rational& right){
        *this += -right;
        return *this;
    }

    Rational& operator *=(const Rational& right){
        *this = *this * right;
        return *this;
    }

    Rational&operator /=(const Rational& right){
        *this = *this / right;
        return *this;
    }

    //postfix x++
    const  Rational operator ++(int){
        Rational result = *this;
        *this +=1;
        return  result;
    }

    //postfix x--
    const Rational operator --(int){
        Rational result = *this;
        *this -= 1;
        return result;
    }

    //prefix ++x
    Rational& operator ++(){
        *this += 1;
        return *this;
    }

    //prefix --x
    Rational&operator --(){
        *this -= 1;
        return *this;
    }

    friend bool operator < (const Rational& left, const Rational& right);

    friend Rational operator *(const Rational& left, const Rational& right);

    friend Rational operator /(const Rational& left, const Rational& right);

    friend Rational operator /(const Rational& left, const BigInteger& b);

    friend Rational operator /(const BigInteger& a, const Rational& right);

    friend Rational operator /(const int& a, const Rational& right);

    friend Rational operator /(const Rational& left, const int& b);
};

ostream& operator <<(ostream& out, const Rational& right){
    out << right.toString();
    return  out;
}

istream & operator >>(istream& in,Rational& right){
    string str;
    in >> str;

    right = Rational(str);
    return in;
}

bool operator <(const Rational &left, const Rational &right){
    if (!left._sign and right._sign)
        return true;
    else if (left._sign and !right._sign)
        return false;
    else if (left._sign and right._sign)
        return (left.numerator * right.denominator < left.denominator * right.numerator);
    else
        return (-left < -right);
}

bool operator >(const Rational &left, const Rational &right){
    return right < left;
}

bool operator >=(const Rational &left, const Rational &right){
    return !(left < right);
}

bool operator <=(const Rational &left, const Rational &right){
    return !(right < left);
}

bool operator ==(const Rational &left, const Rational &right){
    return !(left > right or left < right);
}

bool operator  !=(const Rational &left, const Rational &right){
    return (left > right or left < right);
}

Rational operator +(const Rational &left, const Rational &right){
    Rational result = left;
    return result += right;
}

Rational operator -(const Rational &left, const Rational &right){
    Rational result = left;
    return  result -= right;
}

Rational operator *(const Rational &left, const Rational &right){
    Rational result;

    result.numerator = left.numerator * right.numerator;
    result.denominator = left.denominator * right.denominator;

    BigInteger temp = Rational::greatest_common_factor(result.numerator, result.denominator);

    result.numerator /= temp;
    result.denominator /= temp;
    if (result.numerator == 0)
        result._sign = true;
    else
        result._sign = !(left._sign xor right._sign);

    return result;
}

Rational operator /(const Rational& left, const Rational& right){
    Rational result;
    result.numerator = left.numerator * right.denominator;
    result.denominator = left.denominator * right.numerator;
    BigInteger temp = Rational::greatest_common_factor(result.numerator, result.denominator);

    result.numerator /= temp;
    result.denominator /= temp;
    if (result.numerator == 0)
        result._sign = true;
    else
        result._sign = !(left._sign xor right._sign);

    return result;
}

Rational operator /(const Rational& left, const BigInteger& b){
    Rational result;
    Rational right = b;
    result.numerator = left.numerator * right.denominator;
    result.denominator = left.denominator * right.numerator;
    BigInteger temp = Rational::greatest_common_factor(result.numerator, result.denominator);

    result.numerator /= temp;
    result.denominator /= temp;
    if (result.numerator == 0)
        result._sign = true;
    else
        result._sign = !(left._sign xor right._sign);

    return result;
}

Rational operator /(const BigInteger& a, const Rational& right){
    Rational result;
    Rational left = a;
    result.numerator = left.numerator * right.denominator;
    result.denominator = left.denominator * right.numerator;
    BigInteger temp = Rational::greatest_common_factor(result.numerator, result.denominator);

    result.numerator /= temp;
    result.denominator /= temp;
    if (result.numerator == 0)
        result._sign = true;
    else
        result._sign = !(left._sign xor right._sign);

    return result;
}

Rational operator /(const Rational& left, const int& b){
    Rational result;
    Rational right = b;
    result.numerator = left.numerator * right.denominator;
    result.denominator = left.denominator * right.numerator;
    BigInteger temp = Rational::greatest_common_factor(result.numerator, result.denominator);

    result.numerator /= temp;
    result.denominator /= temp;
    if (result.numerator == 0)
        result._sign = true;
    else
        result._sign = !(left._sign xor right._sign);

    return result;
}

Rational operator /(const int& a, const Rational& right){
    Rational result;
    Rational left = a;
    result.numerator = left.numerator * right.denominator;
    result.denominator = left.denominator * right.numerator;
    BigInteger temp = Rational::greatest_common_factor(result.numerator, result.denominator);

    result.numerator /= temp;
    result.denominator /= temp;
    if (result.numerator == 0)
        result._sign = true;
    else
        result._sign = !(left._sign xor right._sign);

    return result;
}
