#include <vector>
#include <string>
#include <iostream>


class BigInteger{
private:
    static const int base = 10000;
    std::vector<int> num;
    bool isNegative = false;

    int size() const{
        return num.size();
    }

    int digit(int i) const{
        if(size() > i)
            return num[i];
        else
            return 0;
    }

    static BigInteger simpleMult(const BigInteger& first, const BigInteger& second){
        BigInteger res;
        if((first.digit(0) == 0 && first.size() <= 1) || (second.digit(0) == 0 && second.size() <= 1)){
            res = 0;
            return res;
        }
        res.isNegative = first.isNegative ^ second.isNegative;
        int mem = 0;
        for(int i = 0; i < second.size(); ++i){
            for(int j = 0; j < first.size() || mem > 0; ++j){
                if(res.size() <= i + j){
                    res.num.push_back(0);
                }
                res.num[i + j] += first.digit(j) * second.digit(i) + mem;
                mem = res.num[i + j] / base;
                res.num[i + j] %= base;
            }
        }
        return res;
    }

    BigInteger devide(){
        BigInteger first = *this;
        int mem = 0;
        for(int i = first.size() - 1; i > -1; --i){
            int temp = (first.num[i] + mem) / 2;
            (first.num[i] % 2 == 1)? mem = base : mem = 0;
            first.num[i] = temp;
        }

        while(first.size() > 1 && first.digit(first.size() - 1) == 0)
            first.num.erase(first.num.begin() + (first.size() - 1));

        return first;
    }

    void forZerocase(){
        if(size() == 1 && num[0] == 0)
            isNegative = false;
    }
public:

    BigInteger() : BigInteger(0) {}

    BigInteger(int n){
        num.resize(0);
        if(n == 0){
            isNegative = false;
            num.push_back(0);
        } else if(n > 0){
            isNegative = false;
            while(n > 0){
                num.push_back(n % base);
                n /= base;
            }
        }else{
            isNegative = true;
            n *= -1;
            while(n > 0){
                num.push_back(n % base);
                n /= base;
            }
        }
    }

    BigInteger(const BigInteger& n){
        if(this == &n) return;
        isNegative = n.isNegative;
        num = n.num;
    }

    BigInteger(const std::string s){
        num.resize(1);
        int stp = -1;
        if(s[0] =='-'){
            isNegative = true;
            stp = 0;
        } else {
            isNegative = false;
        }
        int k = 1;
        int pos = 0;
        for(int i = s.size() - 1; i > stp; --i){
            if (k == base) {
                ++pos;
                k = 1;
                num.push_back(0);
            }
            if(k == 1){
                num[pos] = s[i] - '0';
                k *= 10;
            } else if (k < base){
                num[pos] += k * (s[i] - '0');
                k *= 10;
            }
        }
    }

    BigInteger operator -() const{
        BigInteger help = *this;
        help.isNegative = ! this->isNegative;
        help.forZerocase();
        return help;
    }
//NEEDS TO ADD NEG - POS && NEG -NEG
    BigInteger& operator += (const BigInteger& first){
        if(isNegative && !first.isNegative){
            if(-(*this) < first){
                this->isNegative = false;
                int mem = 0;
                for(int i = 0; i < size(); ++i){
                    if (mem + first.digit(i) - this->digit(i) >= 0){
                        this->num[i] = mem + first.digit(i) - this->digit(i);
                        mem = 0;
                    } else {
                        this->num[i] = base + mem + first.digit(i) - this->digit(i);
                        mem = -1;
                    }
                }
                for(int i = size(); i < first.size(); ++i){
                    if (mem + first.digit(i) >= 0){
                        this->num.push_back(mem + first.digit(i));
                        mem = 0;
                    } else {
                        this->num.push_back(base + mem + first.digit(i));
                        mem = -1;
                    }
                }
                while (size() > 1 && this->digit(size() - 1) == 0)
                    this->num.erase(this->num.begin() + (size() - 1));
            } else {
                int mem = 0;
                for(int i = 0; i < first.size(); ++i){
                    if (mem + this->digit(i) - first.digit(i) >= 0){
                        this->num[i] = mem + this->digit(i) - first.digit(i);
                        mem = 0;
                    } else {
                        this->num[i] = base + mem + this->digit(i) - first.digit(i);
                        mem = -1;
                    }
                }
                if(mem < 0){
                    this->num[first.size()] += mem;
                }
                while (size() > 1 && this->digit(size() - 1) == 0)
                    this->num.erase(this->num.begin() + (size() - 1));
            }
        } else if(!isNegative && first.isNegative){
            BigInteger help = first;
            help = -(*this);
            help += -first;
            (*this) = -help;
        } else {
            int mem = 0;
            for(int i = 0; i < std::max(size(), first.size()) || mem != 0; ++i){
                if(i >= size()){
                    num.push_back(0);
                }
                int temp = (digit(i) + first.digit(i) + mem)  % base;
                mem = (digit(i) + first.digit(i) + mem) / base;
                this->num[i] = temp;
            }
        }
        this->forZerocase();
        return *this;
    }

    BigInteger& operator -= (const BigInteger& first){
        *this += -first;
        return *this;
    }

    //postfix
    BigInteger operator++(int){
        BigInteger* next= this;
        *this += 1;
        return *next;
    }

    BigInteger operator--(int){
        BigInteger* next= this;
        *this -= 1;
        return *next;
    }

    //prefix CHANGE!!!!!!!!!!!!!!
    BigInteger& operator++(){
        *this += 1;
        return *this;
    }

    BigInteger& operator--(){
        *this -= 1;
        return *this;
    }

    explicit operator bool() const{
        bool ans;
        digit(0) == 0 ? ans = false : ans = true;
        return ans;
    }

    std::string toString() const{
        std::string ans;
        std::string ans1;
        if(isNegative)
            ans += '-';
        for(int i = size() - 1; i > -1; --i){
            if(i == size() - 1){
                ans += std::to_string(num[i]);
            } else {
                ans1 = std::to_string(num[i] + base);
                for(int j = 1; j < 5; ++j){
                    ans += ans1[j];
                }
            }
        }
        return ans;
    }


    friend bool operator < (const BigInteger& first, const BigInteger& second);
    friend bool operator < (const int& a1, const BigInteger& second);
    friend bool operator < (const BigInteger& first, const int& a1);
    friend BigInteger operator * (const BigInteger& a1, const BigInteger& b1);
    friend BigInteger operator / (const BigInteger& first, const BigInteger& second);

    BigInteger& operator *=(const BigInteger& first){
        *this = (*this) * first;
        return *this;
    }

    BigInteger& operator /=(const BigInteger& first){
        *this = *this / first;
        return *this;
    }

    BigInteger& operator %=(const BigInteger& first){
        BigInteger second = (*this / first);
        *this -= second * first;
        return *this;
    }

    BigInteger abs() const{
        BigInteger first = *this;
        first.isNegative = false;
        return first;
    }

};

BigInteger operator + (const BigInteger& first, const BigInteger& second){
    BigInteger sum = second;
    sum += first;
    return sum;
}

BigInteger operator + (const int& first, const BigInteger& second){
    BigInteger sum = first;
    sum += second;
    return sum;
}
BigInteger operator + (const BigInteger& first, const int& second){
    BigInteger sum = second;
    sum += first;
    return sum;
}

BigInteger operator - (const BigInteger& first, const BigInteger& second){
    BigInteger sum = first;
    sum -= second;
    return sum;
}

BigInteger operator - (const int& first, const BigInteger& second){
    BigInteger sum = first;
    sum -= second;
    return sum;
}
BigInteger operator - (const BigInteger& first, const int& second){
    BigInteger sum = first;
    sum -= BigInteger(second);
    return sum;
}

BigInteger operator * (const BigInteger& a1, const BigInteger& b1)  {
    int len = std::max(a1.size(), b1.size());
    BigInteger res;
    res.isNegative = a1.isNegative^b1.isNegative;
    BigInteger first = a1.abs();
    BigInteger second = b1.abs();
    if (len <= 4) {
        return BigInteger::simpleMult(a1, b1);
    }

    int k = (len + 1) / 2;

    BigInteger aL;
    aL.isNegative = false;
    BigInteger aR = 0;
    if(first.size() < k){
        aL.num.resize(first.num.end() - first.num.begin());
        std::copy(first.num.begin(), first.num.end(), aL.num.begin());
    } else {
        aL.num.resize(k);
        aR.num.resize(first.num.end() - first.num.begin() - k);
        std::copy(first.num.begin(), first.num.begin() + k, aL.num.begin());
        std::copy(first.num.begin() + k, first.num.end(), aR.num.begin());
    }

    BigInteger bL;
    bL.isNegative = false;
    BigInteger bR = 0;
    if(second.size() < k){
        bL.num.resize(second.num.end() - second.num.begin());
        std::copy(second.num.begin(), second.num.end(), bL.num.begin());
    } else {
        bL.num.resize(k);
        bR.num.resize(second.num.end() - second.num.begin() - k);
        std::copy(second.num.begin(), second.num.begin() + k, bL.num.begin());
        std::copy(second.num.begin() + k, second.num.end(), bR.num.begin());
    }

    BigInteger aLR = aL + aR;
    BigInteger bLR = bL + bR;

    BigInteger res1 = aL * bL;
    BigInteger res2 = aR * bR;
    BigInteger res3 = aLR * bLR - res1 - res2;

    res.num.resize(4 * k);
    for (int i = 0; i < 2 * k; ++i) {
        res.num[i] = res1.digit(i);
    }

    for (int i = 0; i < 2 * k ; ++i) {
        res.num[i + 2 * k] = res2.digit(i);
    }

    for (int i = 0; i < 2 * k + 1; ++i) {
        res.num[i + k] += res3.digit(i);
    }

    res.num.push_back(0);
    for(int i = 0; i < 2*len; ++i){
        res.num[i + 1] += res.num[i] / BigInteger::base;
        res.num[i] %= BigInteger::base;
    }

    while (res.size() > 1 && res.digit(res.size() - 1) == 0)
        res.num.erase(res.num.begin() + (res.size() - 1));

    res.forZerocase();
    return res;
}

BigInteger operator % (const BigInteger& first, const BigInteger& second){
    BigInteger a1 = first;
    a1 %= second;
    return a1;
}

std::ostream& operator << (std::ostream& of, const BigInteger& first){
    of << first.toString();
    return of;
}

std::istream& operator >> (std::istream& inf, BigInteger& first){
    std::string s = "";
    inf >> s;
    first = BigInteger(s);
    return inf;
}

bool operator < (const BigInteger& first, const BigInteger& second){
    if(first.isNegative && !second.isNegative){
        return true;
    } else if(second.isNegative && !first.isNegative){
        return false;
    } else if(!first.isNegative && !second.isNegative){
        if(first.size() < second.size()){
            return true;
        } else if (first.size() > second.size()){
            return false;
        } else {
            for(int i = first.size() - 1; i > -1; --i){
                if (first.digit(i) < second.digit(i))
                    return true;
                if (first.digit(i) > second.digit(i))
                    return false;
            }
            return false;
        }
    } else {
        return (-second < -first);
    }
}

bool operator > (const BigInteger& first, const BigInteger& second){
    return second < first;
}

bool operator <= (const BigInteger& first, const BigInteger& second){
    return !(first > second);
}

bool operator >= (const BigInteger& first, const BigInteger& second){
    return !(first < second);
}

bool operator == (const BigInteger& first, const BigInteger& second){
    return !(first > second || first < second);
}

bool operator != (const BigInteger& first, const BigInteger& second){
    return (first > second || first < second);
}

bool operator < (const int& a3, const BigInteger& second){
    BigInteger first = a3;
    if(first.isNegative && !second.isNegative){
        return true;
    } else if(second.isNegative && !first.isNegative){
        return false;
    } else if(!first.isNegative && !second.isNegative){
        if(first.size() < second.size()){
            return true;
        } else if (first.size() > second.size()){
            return false;
        } else {
            for(int i = first.size() - 1; i > -1; --i){
                if (first.digit(i) < second.digit(i))
                    return true;
                if (first.digit(i) > second.digit(i))
                    return false;
            }
            return false;
        }
    } else {
        return (-second < -first);
    }
}
bool operator < (const BigInteger& first, const int& a1){
    BigInteger second = a1;
    if(first.isNegative && !second.isNegative){
        return true;
    } else if(second.isNegative && !first.isNegative){
        return false;
    } else if(!first.isNegative && !second.isNegative){
        if(first.size() < second.size()){
            return true;
        } else if (first.size() > second.size()){
            return false;
        } else {
            for(int i = first.size() - 1; i > -1; --i){
                if (first.digit(i) < second.digit(i))
                    return true;
                if (first.digit(i) > second.digit(i))
                    return false;
            }
            return false;
        }
    } else {
        return (-second < -first);
    }
}
bool operator > (const int& first, const BigInteger& second){
    return second < first;
}
bool operator > (const BigInteger& first, const int& second){
    return second < first;
}
bool operator <= (const int& first, const BigInteger& second){
    return !(first > second);
}
bool operator <= (const BigInteger& first, const int& second){
    return !(first > second);
}
bool operator >= (const int& first, const BigInteger& second){
    return !(first < second);
}
bool operator >= (const BigInteger& first, const int& second){
    return !(first < second);
}
bool operator == (const int& first, const BigInteger& second){
    return !(first > second || first < second);
}
bool operator == (const BigInteger& first, const int& second){
    return !(first > second || first < second);
}
bool operator != (const BigInteger& first, const int& second){
    return (first > second || first < second);
}
bool operator != (const int& first, const BigInteger& second){
    return (first > second || first < second);
}

BigInteger operator / (const BigInteger& first, const BigInteger& second){
    if (first == 0) {
        return BigInteger(0);
    }

    BigInteger carry = 0;
    BigInteger abs = first.abs();
    BigInteger sabs = second.abs();

    if (abs < sabs) {
        return BigInteger(0);
    } else if (abs > sabs) {
        BigInteger left = 1;
        BigInteger right = abs + 1;
        BigInteger result = (left + right).devide();

        while (left < --right) {
            ++right;
            result = (left + right).devide();

            BigInteger temp = result * sabs;
            if(temp < abs) {
                left = result;
            } else if (temp > abs) {
                right = result;
            } else {
                right = result;
                break;
            }
        }

        carry = abs - right * sabs;
        BigInteger out = right;
        out.isNegative = first.isNegative ^ second.isNegative;
        if (out.isNegative && (carry != 0)) {
            --out;
        }

        return out;
    } else {
        BigInteger result = BigInteger(1);
        result.isNegative = first.isNegative ^ second.isNegative;
        return result;
    }
}



class Rational{
private:
    static const int base = 10000;
    BigInteger  num;
    BigInteger den;
    bool isNegative = false;

    static BigInteger gcd(const BigInteger& first, const BigInteger& second){
        return (second != 0 ? gcd(second, first % second) : first);
    }

public:

    Rational() = default;

    Rational(int n){
        if(n == 0){
            isNegative = false;
            num = 0;
            den = 1;
        } else if(n > 0){
            isNegative = false;
            num = n;
            den = 1;
        }else{
            isNegative = true;
            num = -n;
            den = 1;
        }
    }
    Rational(BigInteger n){
        if(n == 0){
            isNegative = false;
            num = 0;
            den = 1;
        } else if(n > 0){
            isNegative = false;
            num = n;
            den = 1;
        }else{
            isNegative = true;
            num = -n;
            den = 1;
        }
    }

    Rational(const Rational& n){
        isNegative = n.isNegative;
        num = n.num;
        den = n.den;
    }

    Rational(const std::string str){
        std::string n = "",  divan= "";
        unsigned int i;
        if(str[0] == '-'){
            isNegative = true;
            i = 1;
        } else {
            isNegative = false;
            i = 0;
        }
        for(; str[i] != '/' && i < str.size(); ++i){
            n += str[i];
        }
        ++i;
        for(; i < str.size(); ++i){
            divan += str[i];
        }
        num = BigInteger(n);
        den = BigInteger(divan);
        BigInteger temp = gcd(num, den);
        num /= temp;
        den /= temp;
    }

    Rational operator -() const{
        Rational help = *this;
        help.isNegative = ! this->isNegative;
        return help;
    }

    Rational& operator += (const Rational& first){
        if(isNegative && !first.isNegative){
            Rational help = -(*this);
            help += -first;
            *this = -help;
        } else if(!isNegative && first.isNegative){
            BigInteger temp = den;
            num *= first.den;
            den *= first.den;
            num -= first.num * temp;
            if(num < BigInteger(0)){
                isNegative = true;
                num = -num;
            }
            temp = gcd(num, den);
            num /= temp;
            den /= temp;
        } else {
            BigInteger temp = den;
            num *= first.den;
            den *= first.den;
            num += first.num * temp;
            temp = gcd(num, den);
            num /= temp;
            den /= temp;
        }
        return *this;
    }

    Rational& operator -= (const Rational& first){
        *this += -first;
        return *this;
    }
    Rational& operator -= (const BigInteger& first){
        *this += -first;
        return *this;
    }

    //postfix
    const Rational operator++(int){
        Rational* next= this;
        *this += 1;
        return *next;
    }

    const Rational operator--(int){
        Rational* next= this;
        *this -= Rational(1);
        return *next;
    }

    //prefix
    Rational& operator++(){
        *this += 1;
        return *this;
    }

    Rational& operator--(){
        *this -= Rational(1);
        return *this;
    }

    std::string asDecimal(size_t precision=0){
        BigInteger integ = (num / den);
        BigInteger div = num % den;
        std::string frac = "";
        std::string help;
        while (frac.size() <= precision) {
            help = ((div * BigInteger(base)) / den).toString();
            for(unsigned int i = 0; i < 4 - help.size(); ++i){
                frac += '0';
            }
            frac += help;
            div = (div * BigInteger(base)) % den;
        }
        int i = precision;
        if(frac[i] - '4' > 0){
            --i;
            while((i > -1) && (frac[i] == '9')){
                frac[i] ='0';
                --i;
            }
            if(i == -1)
                ++integ;
            else
                frac[i] = frac[i] + 1;
        }
        if(precision == 0 && frac[0] - '4' > 0){
            ++integ;
        }
        std::string ans = "";
        if(isNegative){
            ans += '-';
        }
        ans += integ.toString()+".";
        for(size_t i = 0; i < precision; ++i){
            ans += frac[i];
        }
        return ans;
    }

    explicit operator double(){
        std::string help = asDecimal(300);
        BigInteger integ = num / den;
        if(help[0] == '-')
            integ *= -1;
        return stod(help);

    }

    explicit operator bool(){
        return (bool(num));
    }

    std::string toString() const{
        std::string ans = "";
        if(isNegative) ans += '-';
        ans += num.toString();
        if(den != 1){
            ans += '/';
            ans += den.toString();
        }
        return ans;
    }

    friend bool operator < (const Rational& first, const Rational& second);
    friend Rational operator * (const Rational& first, const Rational& second);
    friend Rational operator / (const Rational& first, const Rational& second);
    friend Rational operator / (const Rational& first, const int& b1);
    friend Rational operator / (const int& a1, const Rational& second);
    friend Rational operator / (const BigInteger& a1, const Rational& second);
    friend Rational operator / (const Rational& first, const BigInteger& b1);
    friend bool operator < (const Rational& first, const int& b1);
    friend bool operator < (const int& b1, const Rational& second);
    friend bool operator < (const Rational& first, const BigInteger& b1);
    friend bool operator < (const BigInteger& b1, const Rational& second);

    Rational& operator *=(const Rational& first){
        *this = (*this) * first;\
        return *this;
    }

    Rational& operator /=(const Rational& first){
        *this = *this / first;
        return *this;
    }


    Rational abs() const{
        Rational first = *this;
        first.isNegative = false;
        return first;
    }
};

Rational operator + (const Rational& first, const Rational& second){
    Rational sum = second;
    sum += first;
    return sum;
}

Rational operator + (const int& first, const Rational& second){
    Rational sum = second;
    sum += first;
    return sum;
}
Rational operator + (const Rational& first, const int& second){
    Rational sum = second;
    sum += first;
    return sum;
}
Rational operator + (const Rational& first, const BigInteger& second){
    Rational sum = second;
    sum += first;
    return sum;
}
Rational operator + (const BigInteger& first, const Rational& second){
    Rational sum = first;
    sum += second;
    return sum;
}

Rational operator - (const Rational& first, const Rational& second){
    Rational sum = first;
    sum -= second;
    return sum;
}

Rational operator - (const int& first, const Rational& second){
    Rational sum = first;
    sum -= second;
    return sum;
}
Rational operator - (const Rational& first, const int& second){
    Rational sum = first;
    sum -= Rational(second);
    return sum;
}

Rational operator * (const Rational& first, const Rational& second){
    Rational ans;
    ans.num = first.num * second.num;
    ans.den = first.den * second.den;
    BigInteger temp = Rational::gcd(ans.num, ans.den);
    ans.num /= temp;
    ans.den /= temp;
    ans.num == 0?ans.isNegative = false: ans.isNegative = first.isNegative ^ second.isNegative;
    return ans;
}

Rational operator / (const Rational& first, const Rational& second){
    Rational ans;
    ans.num = first.num * second.den;
    ans.den = first.den * second.num;
    BigInteger temp = Rational::gcd(ans.num, ans.den);
    ans.num /= temp;
    ans.den /= temp;
    ans.num == 0?ans.isNegative = false: ans.isNegative = first.isNegative ^ second.isNegative;
    return ans;
}
Rational operator / (const int& a1, const Rational& second){
    Rational first = a1;
    Rational ans;
    ans.num = first.num * second.den;
    ans.den = first.den * second.num;
    BigInteger temp = Rational::gcd(ans.num, ans.den);
    ans.num /= temp;
    ans.den /= temp;
    ans.num == 0?ans.isNegative = false: ans.isNegative = first.isNegative ^ second.isNegative;
    return ans;
}
Rational operator / (const Rational& first, const int& b1){
    Rational second = b1;
    Rational ans;
    ans.num = first.num * second.den;
    ans.den = first.den * second.num;
    BigInteger temp = Rational::gcd(ans.num, ans.den);
    ans.num /= temp;
    ans.den /= temp;
    ans.num == 0?ans.isNegative = false: ans.isNegative = first.isNegative ^ second.isNegative;
    return ans;
}
Rational operator / (const BigInteger& a1, const Rational& second){
    Rational first = a1;
    Rational ans;
    ans.num = first.num * second.den;
    ans.den = first.den * second.num;
    BigInteger temp = Rational::gcd(ans.num, ans.den);
    ans.num /= temp;
    ans.den /= temp;
    ans.num == 0?ans.isNegative = false: ans.isNegative = first.isNegative ^ second.isNegative;
    return ans;
}
Rational operator / (const Rational& first, const BigInteger& b1){
    Rational second = b1;
    Rational ans;
    ans.num = first.num * second.den;
    ans.den = first.den * second.num;
    BigInteger temp = Rational::gcd(ans.num, ans.den);
    ans.num /= temp;
    ans.den /= temp;
    ans.num == 0?ans.isNegative = false: ans.isNegative = first.isNegative ^ second.isNegative;
    return ans;
}

std::ostream& operator << (std::ostream& of, const Rational& first){
    of << first.toString();
    return of;
}

std::istream& operator >> (std::istream& inf, Rational& first){
    std::string s = "";
    inf >> s;
    first = Rational(s);
    return inf;
}

bool operator < (const Rational& first, const Rational& second){
    if(first.isNegative && !second.isNegative){
        return true;
    } else if(second.isNegative && !first.isNegative){
        return false;
    } else if(!first.isNegative && !second.isNegative){
        return (first.num * second.den < first.den * second.num);
    } else {
        return (-second < -first);
    }
}
bool operator < (const Rational& first, const int& b1){
    Rational second = b1;
    if(first.isNegative && !second.isNegative){
        return true;
    } else if(second.isNegative && !first.isNegative){
        return false;
    } else if(!first.isNegative && !second.isNegative){
        return (first.num * second.den < first.den * second.num);
    } else {
        return (-second < -first);
    }
}
bool operator < (const int& b1, const Rational& second){
    Rational first = b1;
    if(first.isNegative && !second.isNegative){
        return true;
    } else if(second.isNegative && !first.isNegative){
        return false;
    } else if(!first.isNegative && !second.isNegative){
        return (first.num * second.den < first.den * second.num);
    } else {
        return (-second < -first);
    }
}
bool operator < (const Rational& first, const BigInteger& b1){
    Rational second = b1;
    if(first.isNegative && !second.isNegative){
        return true;
    } else if(second.isNegative && !first.isNegative){
        return false;
    } else if(!first.isNegative && !second.isNegative){
        return (first.num * second.den < first.den * second.num);
    } else {
        return (-second < -first);
    }
}
bool operator < (const BigInteger& b1, const Rational& second){
    Rational first = b1;
    if(first.isNegative && !second.isNegative){
        return true;
    } else if(second.isNegative && !first.isNegative){
        return false;
    } else if(!first.isNegative && !second.isNegative){
        return (first.num * second.den < first.den * second.num);
    } else {
        return (-second < -first);
    }
}

bool operator > (const Rational& first, const Rational& second){
    return second < first;
}

bool operator <= (const Rational& first, const Rational& second){
    return !(first > second);
}

bool operator >= (const Rational& first, const Rational& second){
    return !(first < second);
}

bool operator == (const Rational& first, const Rational& second){
    return !(first > second || first < second);
}

bool operator != (const Rational& first, const Rational& second){
    return (first > second || first < second);
}

bool operator > (const Rational& first, const int& second){
    return second < first;
}
bool operator <= (const Rational& first, const int& second){
    return !(first > second);
}
bool operator >= (const Rational& first, const int& second){
    return !(first < second);
}
bool operator == (const Rational& first, const int& second){
    return !(first > second || first < second);
}
bool operator != (const Rational& first, const int& second){
    return (first > second || first < second);
}

bool operator > (const int& first, const Rational& second){
    return second < first;
}
bool operator <= (const int& first, const Rational& second){
    return !(first > second);
}
bool operator >= (const int& first, const Rational& second){
    return !(first < second);
}
bool operator == (const int& first, const Rational& second){
    return !(first > second || first < second);
}
bool operator != (const int& first, const Rational& second){
    return (first > second || first < second);
}

bool operator > (const Rational& first, const BigInteger& second){
    return second < first;
}
bool operator <= (const Rational& first, const BigInteger& second){
    return !(first > second);
}
bool operator >= (const Rational& first, const BigInteger& second){
    return !(first < second);
}
bool operator == (const Rational& first, const BigInteger& second){
    return !(first > second || first < second);
}
bool operator != (const Rational& first, const BigInteger& second){
    return (first > second || first < second);
}

bool operator > (const BigInteger& first, const Rational& second){
    return second < first;
}
bool operator <= (const BigInteger& first, const Rational& second){
    return !(first > second);
}
bool operator >= (const BigInteger& first, const Rational& second){
    return !(first < second);
}
bool operator == (const BigInteger& first, const Rational& second){
    return !(first > second || first < second);
}
bool operator != (const BigInteger& first, const Rational& second){
    return (first > second || first < second);
}