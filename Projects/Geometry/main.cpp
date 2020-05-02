#include <iostream>
#include "geometry.h"

using namespace std;


int main() {

    Point a(-2, 2), b(1, 2), f(6, 1), c(3, -1), e(1, -1), d(-1, -2), k(3, 1);

    Line ab(a, b);
    Line ba(b, a);
    cout << (ab == ba) << endl;

    Polygon abfe(a, b, f, e);
    Polygon bfea(a, b, f, e);

    cout << (abfe == bfea) << endl;

    Circle fc(f, 4);
    Circle cf (f, 4);

    cout << (fc == cf) << endl;


    return 0;
}
