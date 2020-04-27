#include <iostream>
#include "geometry.h"

using namespace geometry;
using namespace std;

int main() {
    Point point1(3, 0);
    Point point2(-3, 0);
    Point point3(4, 0), point4(3,-2);


    Ellipse X(point1, point2, 10);
    cout << X.center().x << ':' << X.center().y;
    pair<Line, Line> ls= X.directrices();
    cout << 1;
    return 0;
}
