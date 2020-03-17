#include <iostream>
#include <vector>
#include <string>

using namespace std;

class BigInteger{
public:
    BigInteger() = default;

    friend ostream& operator << (ostream &out, BigInteger &BI);

    friend istream& operator >> (istream &in, BigInteger &BI);

private:
    int _sign = 1;
    vector<long int> _bits;
    static const int BASE = 1000000000; //10^9

};

ostream &operator<<(ostream &out, BigInteger &BI) {
    if (BI._sign == -1) cout << "-";
    for(auto i : BI._bits)
        cout << i;
    return out;
}

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

    for (int i = 0; i < (str_number.size() + 8)/ 9; ++i) {
        BI._bits.push_back(stoi(str_number.substr(i*9, 9)));
    }
    return in;
}

int main() {
    BigInteger kak;

    cin >> kak;

    cout << kak;

    return 0;
}
