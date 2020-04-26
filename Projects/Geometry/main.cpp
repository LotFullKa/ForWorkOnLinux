#include <iostream>
#include "geometry.h"

using namespace geometry;
using namespace std;

int main() {
    Point point1(0, 0);
    Point point2(1, 1);
    Point point3(-1, -1);

    cout << (point1 == point2) << endl;

    Line first_line(point1, point2);
    Line second_line(point2, point3);

    std::cout << (second_line == first_line);
    return 0;
}
