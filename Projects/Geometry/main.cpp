#include <iostream>
#include "geometry.h"

using namespace std;


int main() {

    Point a(-3, 0), b(3, 0), c(0,3 * sqrt(3) );
    Triangle abc(a , b, c);
    cout << abc.ninePointsCircle().center().x << endl;
    cout << abc.ninePointsCircle().center().y << endl;
    cout << abc.ninePointsCircle().radius() << endl << endl;


    cout << abc.orthocenter().x << endl;
    cout << abc.orthocenter().y << endl;

    cout << abc.circumscribedCircle().center().x << endl;
    cout << abc.circumscribedCircle().center().y << endl;

    cout << abc.inscribedCircle().center().x << endl;
    cout << abc.inscribedCircle().center().y << endl;

}
