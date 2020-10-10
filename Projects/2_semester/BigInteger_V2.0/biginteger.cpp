#include "biginteger.h"
#include <fstream>

int main() {

    ofstream out("/home/kamil/Experiments/ForWorkOnLinux/Projects/BigInteger_V2.0/out_of_cpp.txt");

    Rational s, q, t;

    s = 4*3*7*13*19*41*43*11;
    t = -17*13*23*79;
    s *= s*s, t *= t*t;
    q = s/t;
    q *= t/s;
    if (q != 1 )
        cout << "error !";
    if (q.toString() != "1")
        cout << "error1";

    BigInteger z ("-1214387438957564646453"), h ("432489385945");
    cout << z/h <<endl;
    cout << -11/5;
    return 0;
}

