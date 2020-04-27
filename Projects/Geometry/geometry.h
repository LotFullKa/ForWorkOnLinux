#include <cmath>
#include <utility>
#include <vector>
#include <stdarg.h>
#include <array>

namespace geometry {
    static long double eps = 1e-9;

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
        // хранит в себе угол поворота вектора
        long double _x;
        long double _y;
        long double angle;

        Vector() {
            _x = 1;
            _y = 0;
            angle = 0;
        };

        Vector (const Point &start, const  Point &finish) {
            _x = finish.x - start.x;
            _y = finish.y - start.y;
            angle_update();
        }

        Vector(const double &x,const double &y) : _x(x), _y(y) {
            angle_update();
        }

        Vector(Point& finish) : _x(finish.x), _y(finish.y) {
            angle_update();
        }

        bool operator ==(Vector other_vec) const {
            return angle == other_vec.angle and get_vector_len() == other_vec.get_vector_len();
        }

        bool operator !=(Vector other_vec) const {
            return !(*this == other_vec);
        }

        Vector& operator +=(Vector& other_vec) {
            _x += other_vec._x;
            _y += other_vec._y;
            angle_update();
            return *this;
        }

        Vector& operator *=(double& lambda) {
            _x *= lambda;
            _y *= lambda;
            angle_update();
            return *this;
        }

        double get_vector_len() const {
            return sqrt(_x*_x + _y*_y);
        }

        Point get_point() const {
            Point result(_x, _y);
            return result;
        }

    private:
        void angle_update() {
            angle = atan(_y/_x);

            if (_x < 0 and _y < 0)
                angle += M_PI;
            else if (_x < 0 and _y > 0)
                angle += M_PI;
            else if (_x > 0 and _y < 0)
                angle += 2*M_PI;
        }
    };

    Vector operator +(Vector f_vec, Vector s_vec){
        Vector result(0,0);
        result += f_vec;
        result += s_vec;
        return result;
    }

    Vector operator *(Vector vec, double lambda) {
        Vector result = (vec *= lambda);
        return result;
    }

    class Line {
    public:
        Line (const Point &start, const Point &finish) : point_on_line(start) {
            slope_factor = get_slope(start, finish);
        }

        Line (const long double &angle, const long double & shift_x) : point_on_line(shift_x, 0), slope_factor(tan(angle)){}

        Line (const Point &start, const long double &angle) : point_on_line(start), slope_factor(tan(angle)){}

        bool operator ==(Line& other_line) const{
            return (slope_factor == other_line.slope_factor and
            get_slope(point_on_line, other_line.point_on_line) == slope_factor);
        }

        bool operator !=(Line& other_line) const {
            return !(*this == other_line);
        }

        double get_slope() {
            return slope_factor;
        }

    protected:
        Point point_on_line;
        double slope_factor;

        double get_slope(const Point &start, const Point &finish) const {
            return (finish.y - start.y) / (finish.x - start.x);
        }
    };

    class Figure {
    public:
        //virtual double perimeter() = 0;
        //virtual double area() = 0;
    };

    class Polygon : Figure {
    public:

        Polygon (std::vector<Point> vertices) : vertices(std::move(vertices)) {}

        template<typename ...ArgsT>
        Polygon (ArgsT... points) {
            std::array<Point, sizeof...(ArgsT)> const poly {points...};

            for (const auto p : poly){
                vertices.push_back(p);
            }
        }

        int verticesCount(){
            return vertices.size();
        }

        std::vector<Point> getVertices () {
            return vertices;
        }

        bool isConvex(){
            if (vertices.size() < 4)
                return true;

            Point first_p = vertices[0];
            Point second_p = vertices[1];
            Point third_p = vertices[2];
            Point fourth_p;
            int flag = check_twist(first_p, second_p, third_p);

            for (int i = 3; i < vertices.size(); ++i){
                fourth_p = vertices[i];
                first_p = second_p;
                second_p = third_p;
                third_p = fourth_p;

                if (flag != check_twist(first_p, second_p, third_p))
                    return false;
            }

            return flag == check_twist(second_p, third_p, vertices[0]);
        }

    private:
        std::vector<Point> vertices;

        int check_twist(const Point& first, const Point& second, const Point& third) {
            //если 1 поворот против часовой стрелки, если  -1 поворот по часовой стрелке
            double angle_one = Vector(first, second).angle;
            double angle_two = Vector(second, third).angle;

            double delta_angle = angle_two - angle_one;
            return fabs(delta_angle) > M_PI xor delta_angle > 0 ? 1 : -1;
        }

    };

    class Ellipse : Figure {
    public:

        Ellipse(const Point& focus_a, const Point& focus_b, double twice_A) :
        focus_a(focus_a),
        focus_b(focus_b),
        A(twice_A / 2)
        {
            C = Vector(focus_a, focus_b).get_vector_len() / 2;
        }

        Point center() {
            Vector v1 = Vector(focus_a, focus_b) * 0.5;
            Vector v2 = Vector(focus_a) + v1;
            return v2.get_point();
        }

        std::pair<Point,Point> focuses() {
            return std::make_pair(focus_a, focus_b);
        }

        double eccentricity() {
            return C / A;
        }

        std::pair<Line, Line> directrices() {
            double dir_slope_factor = 1 / Line(focus_a, focus_b).get_slope();
            Point on_f_directr = (Vector(focus_a,focus_b) * ((A * A) / (C * C * 2))).get_point();
            Point on_s_directr = (Vector(focus_a,focus_b) * ((A * A) / (C * C * -2))).get_point();
            return std::make_pair(Line(on_f_directr, dir_slope_factor), Line(on_s_directr, dir_slope_factor));
        }

    private:
        Point focus_a;
        Point focus_b;
        double A;
        double C;
    };

}