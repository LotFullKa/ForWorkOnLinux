#include <cmath>

namespace geometry {
    static long double eps = 1e-9;

    struct Angle {
        // Представление ang градусах

    private:
        long double ang;

    public:
        Angle(long double ang): ang(fmod((ang * 180/M_PI), (2*M_PI))) {}

        long double get_ang() {
            return ang;
        }
    };

    struct Point {
    public:

        Point(const double x = 0,const double y = 0) : x(x), y(y) {}

        bool operator==(const Point &other) const {
            return !((fabs(x - other.x) or fabs(y - other.y)));
        }

        bool operator!=(const Point &other) const {
            return !(*this == other);
        }

        long double x;
        long double y;
    };

    struct Vector {
        // единичный вектор хранящий в себе угловой коэф
        long double _x = 1;
        long double _y = 1;
        long double _k;

        Vector (const Point &start, const  Point &finish) {
            _x = finish.x - start.x;
            _y = finish.y - start.y;
            _k = _y / _x;
        }

        Vector (const long double angle ) : _k(tan(angle)) {}

        Vector(const double &x = 0,const double &y = 0) : _x(x), _y(y), _k(_y / _x) {}

        bool operator ==(Vector other_vec) const {
            return _k == other_vec._k;
        }

        bool operator !=(Vector other_vec) const {
            return _k != other_vec._k;
        }

    };

    class Line {
    public:
        Line (const Point &start, const Point &finish) : point_on_line(start), main_direction(start, finish) {}

        Line (const long double &angle, const long double & shift_x) : point_on_line(shift_x), main_direction(angle) {}

        Line (const Point &start, const long double &angle) : point_on_line(start), main_direction(angle) {}

        bool operator ==(Line& other_line) const{
            return (main_direction == other_line.main_direction and
            Vector(point_on_line, other_line.point_on_line) == main_direction);
        }

        bool operator !=(Line& other_line) const {
            return !(*this == other_line);
        }

    protected:
        Point point_on_line;
        Vector main_direction;
    };

    class Figure {
    public:
        virtual void i_virtual_func(int x) = 0;
    };

    class Polygon : 
}