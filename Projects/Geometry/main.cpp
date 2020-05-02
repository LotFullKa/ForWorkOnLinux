#include <iostream>
#include "geometry.h"

using namespace std;

int main() {
    Point p1(0, 0);
    Point p2(4, 0);
    Point p3(5, 6),
    p4(0, 4),
    p5(1, -3),
    p6(4, 1);

    Polygon ak(p1, p2, p3, p4);

    ak.scale(p1, 2);
    cout << ak.getVertices()[1];

    Circle cr(p3, 3);
    cr.reflex(Point(0,0));


    cout << cr;

    return 0;
}
