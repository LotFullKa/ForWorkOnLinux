#include <iostream>
#include "geometry.h"

using namespace geometry;
using namespace std;

int main() {
    //cout.precision(9);
    //cout.setf(ios::fixed);
    Point point1(0, 0);
    Point point2(3, 0);
    Point point3(0, 4), point4(5,-1);

    Triangle sq(point1, point2, point3);
    cout << sq.inscribedCircle().center() << endl;

    return 0;
}
