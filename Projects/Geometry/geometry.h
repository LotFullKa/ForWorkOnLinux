#include <cmath>
#include <utility>
#include <vector>
#include <stdarg.h>
#include <array>
#include <math.h>

namespace geometry {
    static double eps = 1e-9;
    double fixed (double num){
        return round(num);
    }

    struct Point {
    public:

        Point(const double x = 0,const double y = 0) : x(x), y(y) {}

        bool operator==(const Point &other) const {
            return !((fabs(x - other.x) < eps or fabs(y - other.y) < eps));
        }

        bool operator!=(const Point &other) const {
            return !(*this == other);
        }

        double x;
        double y;
    };

    std::ostream& operator <<(std::ostream& out, const Point& P){
        out << P.x << " : " << P.y;
        return out;
    }

    struct Vector {
        // хранит в себе угол поворота вектора в радианах [0, 2Pi)
        double _x;
        double _y;
        double angle;

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

        Vector(const Point& finish) : _x(finish.x), _y(finish.y) {
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

        Vector& operator *=(double lambda) {
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

        void rotate(double plus_angle) {
            //поворот по часовой на угол angle
            double len = get_vector_len();
            _x = len * cos(angle + plus_angle);
            _y = len * sin(angle + plus_angle);
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
        Line (const Point &start, const Point &finish) : point_on_line(start), second_on_line(finish) {
            slope_factor = get_slope(start, finish);
        }

        Line (const double &angle, const double & shift_x) :
                point_on_line(shift_x, 0),
                slope_factor(tan(angle))
                {
                    second_on_line = (Vector(point_on_line) + Vector(1, slope_factor)).get_point();
                }

        Line (const Point &start, const double &angle) :
                point_on_line(start),
                slope_factor(tan(angle)) {
                    second_on_line = (Vector(start) + Vector(1, slope_factor)).get_point();
                }

        Line (const Point &start,const Vector &vec) :
                point_on_line(start),
                slope_factor(tan(vec.angle)),
                second_on_line((Vector(start) + vec).get_point())
                {}

        bool operator ==(Line& other_line) const{
            return (slope_factor == other_line.slope_factor and
            get_slope(point_on_line, other_line.point_on_line) == slope_factor);
        }

        bool operator !=(Line& other_line) const {
            return !(*this == other_line);
        }

        Point operator ^(Line other_line) {
            double intersection_x =(get_free_member() - other_line.get_free_member())/(other_line.slope_factor - slope_factor);
            Point result(intersection_x, intersection_x * slope_factor);
            return  result;
        }

        double get_slope() const {
            return slope_factor;
        }

        Line get_perpendicular(const Point& on_line) const {
            Line result(on_line, atan(slope_factor) + M_PI/2);
            return result;
        }

    protected:
        Point point_on_line;
        Point second_on_line;
        double slope_factor;

        static double get_slope(const Point &start, const Point &finish) {
            return (finish.y - start.y) / (finish.x - start.x);
        }

        double get_free_member() const {
            return point_on_line.y - slope_factor * point_on_line.x;
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

        double perimeter() const {
            double perimeter = 0;
            for (int i = 0; i < vertices.size() - 1; ++i) {
                perimeter += Vector(vertices[i], vertices[i + 1]).get_vector_len();
            }
            perimeter += Vector(vertices[0], vertices[vertices.size() - 1]).get_vector_len();
            return perimeter;
        }

        double area() {
            double Gauss_sum = vertices[vertices.size() - 1].x * vertices[0].y - vertices[0].x * vertices[vertices.size() - 1].y;
            for (int i = 0; i < vertices.size() - 1; ++i) {
                Gauss_sum += vertices[i].x * vertices[i + 1].y;
                Gauss_sum -= vertices[i + 1].x * vertices[i].y;
            }
            return fabs(Gauss_sum) * 0.5;
        }

    protected:
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
            return (Vector(focus_a) + Vector(focus_a, focus_b) * 0.5).get_point();
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

    class Circle : public Ellipse {
    public:
        Circle(const Point& center, double radius) :
                Ellipse(center, center, radius * 2),
                cent(center),
                rad(radius)
        {}

        double radius() const {
            return rad;
        }

    private:
        Point cent;
        double rad;
    };

    class Rectangle : public Polygon {
    public:

        Rectangle(const Point& left_down,const Point& right_up, double ratio) : Polygon(), ratio(ratio) {
            if (ratio < 1)
                ratio = 1 / ratio;

            vertices.push_back(left_down);

            Vector sicker(left_down, right_up);
            sicker.rotate(atan(ratio));
            sicker *= (1/sqrt(ratio * ratio + 1));

            Point left_up = (Vector(left_down) + sicker).get_point();
            vertices.push_back(left_up);

            vertices.push_back(right_up);

            Point right_down = (Vector(left_down) + Vector(left_up, right_up)).get_point();
            vertices.push_back(right_down);
        }

        Point center() {
            return ((Vector(vertices[0], vertices[2]) * 0.5) + Vector(vertices[0])).get_point();
        }

        std::pair<Line, Line> diagonals() {
            return std::make_pair(Line(vertices[0], vertices[2]), Line(vertices[1], vertices[3]));
        }

    private:
        double ratio;
    };

    class Square : public Rectangle {
    public:
        Square (const Point& left_down,const Point& right_up) : Rectangle(left_down, right_up, 1){}

        Circle circumscribedCircle() {
            Circle result(center(), (Vector(vertices[0], vertices[2])).get_vector_len() / 2);
            return result;
        }

        Circle inscribedCircle() {
            Circle result(center(), (Vector(vertices[0], vertices[1])).get_vector_len() / 2);
            return result;
        }
    };

    class Triangle : public Polygon {
    public:
        Triangle(const Point& point1, const Point& point2, const Point& point3) : Polygon(point1, point2, point3) {}

        Circle circumscribedCircle() {
           Point center = get_mid_perpendicular(0) ^ get_mid_perpendicular(1);
           double a = Vector(vertices[0], vertices[1]).get_vector_len();
           double b = Vector(vertices[1], vertices[2]).get_vector_len();
           double c = Vector(vertices[2], vertices[0]).get_vector_len();
           double r = a * b * c / (4 * area());
           Circle result(center, r);
           return result;
        }

        Circle inscribedCircle() {
            Point center = (get_bisector(0) ^ get_bisector(1));
            double r = 2 * area() / perimeter();
            Circle result(center, r);
            return result;
        }

        Point orthocenter() {
            Point result = get_triangle_height(0) ^ get_triangle_height(1);
            return result;
        }

        Point centroid() {
            Point result;
            result.x = (vertices[0].x + vertices[1].x + vertices[2].x) / 3;
            result.y = (vertices[0].y + vertices[1].y + vertices[2].y) / 3;
            return result;
        }

        Line EulerLine() {
            Line result(orthocenter(), circumscribedCircle().center());
            return result;
        }

        Circle ninePointsCircle() {
            Point center = ((Vector(orthocenter(), circumscribedCircle().center()) * 0.5)+ Vector(orthocenter())).get_point();
            Point middle = ((Vector(vertices[0], vertices[1]) * 0.5) + Vector(vertices[0])).get_point();
            double r = (Vector(center, middle)).get_vector_len();
            Circle result(center, r);
            return result;
        }

    protected:
        Line get_bisector(const int& vex) {
            //возращает биссектрису из вершины vex (0 - 1 - 2)

            Vector help_vec(vertices[vex], vertices[(vex + 1) % 3]);
            help_vec.rotate(- Vector(vertices[vex], vertices[(vex + 2) % 3]).angle);
            help_vec.rotate(- help_vec.angle);
            Line bisector(vertices[0], help_vec);
            return bisector;
        }

        Line get_mid_perpendicular(const int& vex) {
            //возращает линию середнного перпендикуляра проходящего через прямую напротив вершины vex (0 - 1 - 2)
            Point middle = ((Vector(vertices[(vex + 1) % 3], vertices[(vex + 2) % 3]) * 0.5) + Vector(vertices[(vex + 1) % 3])).get_point();
            Line result(vertices[(vex + 1) % 3], vertices[(vex + 2) % 3]);
            result = result.get_perpendicular(middle);
            return result;
        }

        Line get_triangle_height(const int& vex) {
            //возращает линию совпадаюущю с перпендикуляром из вершины vex(0 - 1 - 2)
            Line result(vertices[(vex + 1) % 3], vertices[(vex + 2) % 3]);
            result = result.get_perpendicular(vertices[vex]);
            return result;
        }
    };

}