#include "biginteger.h"
#include <fstream>

int main() {

    ofstream out("/home/kamil/Experiments/ForWorkOnLinux/Projects/BigInteger_V2.0/out_of_cpp.txt");

    BigInteger a, b;

    cin >> a >> b;

    out << a << endl << b << endl;

    cout << a*b;

    out << a *b;
   Rational s, t;

    s = 4*3*7*13*19*41*43*11;
    t = s - 25; // t=402365939
    ((s = 1000000007) *= 1000000009) *= 2147483647;
    if ((s/t).asDecimal(10) != "5337140829307966068.3989202202")
        std::cout <<4<< std::endl;
    t = -t;
    if ((t/s).asDecimal(25) != "-0.0000000000000000001873662")
        std::cout << 5 << std::endl;

    return 0;
}

