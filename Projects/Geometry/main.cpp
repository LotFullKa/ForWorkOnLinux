#include <iostream>
#include "geometry.h"

using namespace std;

int main() {
    Point point1(0, 0);
    Point point2(4, 0);
    Point point3(0, 3), point4(5,-1);

    Polygon a(point1, point3, point2, point4);
    Polygon b(point2, point4, point1, Point(0, 1));
    cout << (a == b);


    return 0;
}
