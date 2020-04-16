#include <iostream>
#include "geometry.h"

int main() {
    Point point1(1, 3);
    Point point2(2, 3);

    std::cout << (point1 == point2) << std::endl;
    std::cout << (point1 != point2) << std::endl;
    return 0;
}
