#include <cmath>
#include <utility>
#include <vector>
#include <stdarg.h>
#include <array>
#include <math.h>


    static double eps = 1e-9;

    bool isEqual(const double& a,const double& b){
        return  fabs(a - b) < eps;
    }

    struct Point {
    public:

        Point(const double x = 0,const double y = 0) : x(x), y(y) {}

        bool operator==(const Point &other) const {
            return ((fabs(x - other.x) < eps and fabs(y - other.y) < eps));
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
        double _x = 1;
        double _y = 0;
        double angle = 0;

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

        Vector(const double& angle) {
            rotate(angle);
        }

        bool operator ==(Vector other_vec) const {
            return isEqual(angle, other_vec.angle) and isEqual(get_vector_len() , other_vec.get_vector_len());
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
            angle_update();
        }

    private:


        void angle_update() {
            angle = atan(_y/_x);

            if (_x < 0)
                angle += M_PI;
            else if (_x >= 0 and _y < 0)
                angle += 2*M_PI;
        }
    };

    struct Angle {
        // вспомогательная структура строящая угол от трех точек
        // угол от on_line1 - ver - on_line2
        Angle(const Point& ver, const Point& on_line1, const Point& on_line2) {
            double a1 = Vector(ver, on_line1).angle;
            double a2 = Vector(ver, on_line2).angle;
            angle = fmin(abs(a1 - a2), 2*M_PI - abs(a1 - a2));
        }

        double get_angle(){
            //вернет градус в радианах
            return angle;
        }

    private:
        double angle;
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
        Line (const Point &start, const Point &finish) : point_on_line(start), second_on_line(finish) {}

        Line (const double &angle, const double & shift_x) :
                point_on_line(shift_x, 0) {
                    second_on_line = (Vector(point_on_line) + Vector(angle)).get_point();
                }

        Line (const Point &start, const double &angle) :
                point_on_line(start) {
                    second_on_line = (Vector(start) + Vector(angle)).get_point();
                }

        Line (const Point &start,const Vector &vec) :
                point_on_line(start),
                second_on_line((Vector(start) + vec).get_point())
                {}

        bool operator ==(Line& other_line) const{
            return isEqual(get_slope() , other_line.get_slope()) and
                    isEqual(get_slope(point_on_line, other_line.point_on_line), get_slope());
        }

        bool operator !=(Line& other_line) const {
            return !(*this == other_line);
        }

        Point operator ^(Line other_line) {
            if (is_verticale())
                return point_on_line.x * other_line.get_slope();
            else if (other_line.is_verticale()) {
                return other_line.point_on_line.x * get_slope();
            } else {
                double intersection_x =
                        (get_free_member() - other_line.get_free_member()) / (other_line.get_slope() - get_slope());
                Point result(intersection_x, intersection_x * get_slope());
                return result;
            }
        }

        double get_slope() const {
            return get_slope(point_on_line, second_on_line);
        }

        Line get_perpendicular(const Point& on_line) const {
            if (is_verticale()) {
                Line result(on_line, 0);
                return result;
            } else {
                Line result(on_line, atan(get_slope()) + M_PI / 2);
                return result;
            }
        }

        //вернет точку симметричную относительно прямой
        Point get_reflex(Point p) {
            Line guide_line = get_perpendicular(p);
            Point intersection = *this ^ guide_line;
            Vector help_vector(p, intersection);
            Point result = (intersection + help_vector).get_point();
            return result;
        }

    protected:
        Point point_on_line;
        Point second_on_line;

        bool is_verticale() const {
            return isnan(get_slope());
        }


        static double get_slope(const Point &start, const Point &finish) {
            if (fabs(finish.x - start.x) < eps)
                return nan("line is verticale");
            return (finish.y - start.y) / (finish.x - start.x);
        }

        double get_free_member() const {
            if (is_verticale())
                return point_on_line.x;
            return point_on_line.y - get_slope() * point_on_line.x;
        }
    };

    class Shape {
    public:
        virtual double area() = 0;
        virtual double perimeter() = 0;


        virtual bool operator ==(const Shape& another) = 0;
        virtual bool isCongruentTo(const Shape& another) = 0;
        virtual bool isSimilarTo(const Shape& another) = 0;
        virtual bool containsPoint(Point point) = 0;

        virtual void rotate(Point center, double angle) = 0;
        virtual void reflex(Point center) = 0;
        virtual void reflex(Line axis) = 0;
        virtual void scale(Point center, double coefficient) = 0;

    };

    class Polygon : public Shape {
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

        double perimeter() override {
            double perimeter = 0;
            for (int i = 0; i < vertices.size() - 1; ++i) {
                perimeter += Vector(vertices[i], vertices[i + 1]).get_vector_len();
            }
            perimeter += Vector(vertices[0], vertices[vertices.size() - 1]).get_vector_len();
            return perimeter;
        }

        double area() override {
            double Gauss_sum = vertices[vertices.size() - 1].x * vertices[0].y - vertices[0].x * vertices[vertices.size() - 1].y;
            for (int i = 0; i < vertices.size() - 1; ++i) {
                Gauss_sum += vertices[i].x * vertices[i + 1].y;
                Gauss_sum -= vertices[i + 1].x * vertices[i].y;
            }
            return fabs(Gauss_sum) * 0.5;
        }

        bool operator ==(const Shape& another) override {
            Polygon an = dynamic_cast<const Polygon &> (another);

            if (vertices.size() != an.vertices.size())
                return false;

            int start_ver;
            bool flag = false;
            for (int i = 0; i < vertices.size(); ++i) {
                if (vertices[0] == an.vertices[i]){
                    start_ver = i;
                    flag = true;
                    break;
                }
            }

            if (!flag)
                return false;

            for (int i = 0; i < vertices.size(); ++i) {
                if (vertices[i] != an.vertices[(start_ver + i) % vertices.size()]){
                    flag = false;
                    break;
                }
            }

            if (flag)
                return  true;

            for (int i = 0; i < vertices.size(); ++i) {
                if (vertices[i] != an.vertices[vertices.size() - (start_ver + i) % vertices.size()]){
                    flag = false;
                    break;
                }
            }

            return flag;
        }

        bool isCongruentTo(const Shape& another) override {
            Polygon an = dynamic_cast<const Polygon &> (another);

            if (!isSimilarTo(an))
                return false;

            return isEqual(area(), an.area());
        }

        bool isSimilarTo(const Shape& another) override {
            Polygon an = dynamic_cast<const Polygon &> (another);

            if(verticesCount() != an.verticesCount())
                return false;

            std::vector<double> angles;
            std::vector<double> an_angles;
            for (int i = 0; i < verticesCount(); ++i) {
                angles.push_back(Angle(vertices[i], vertices[(verticesCount() + i - 1) % verticesCount()], vertices[(i + 1) % verticesCount()]).get_angle());
                an_angles.push_back(Angle(an.vertices[i], an.vertices[(verticesCount() + i - 1) % verticesCount()], an.vertices[(i + 1) % verticesCount()]).get_angle());
            }

            //ловим совпадающие углы, чтобы было с чего начать
            std::vector<int> points_for_veiw;
            for (int i = 0; i < vertices.size(); ++i) {
                if (isEqual(angles[i], an_angles[0]))
                    points_for_veiw.push_back(i);
            }

            bool flag1 = true;
            bool flag2 = true;
            for (int ver : points_for_veiw) {
                //обход в одну сторону
                for (int i = 0; i < verticesCount(); ++i) {
                    if (!isEqual(angles[(ver + i) % verticesCount()], an_angles[i])) {
                        flag1 =  false;
                        break;
                    }
                }

                //обход в другую сторону
                for (int i = 0; i < verticesCount(); ++i) {
                    if (isEqual(angles[(ver + i) % verticesCount()], an_angles[(verticesCount() - i) % verticesCount()])) {
                        flag2 =  false;
                        break;
                    }
                }

                if (flag1 or flag2)
                    return true;
            }
            return false;
        }

        bool containsPoint(Point point) override  {
            double sum = 0;
            for (int i = 0; i < verticesCount(); ++i) {
                sum += Polygon(vertices[i], vertices[(i + 1) % verticesCount()], point).area();
            }

            return isEqual(sum, area());
        }

        void rotate(Point center, double angle) override {
            for (int i = 0; i < verticesCount(); ++i) {
                Vector help_vector(center, vertices[i]);
                help_vector.rotate(angle * M_PI / 180);
                vertices[i] = (center + help_vector).get_point();
            }
        }

        void reflex(Point center) override {
            Vector help_vetor(1, 1);
            for (int i = 0; i < verticesCount(); ++i) {
                help_vetor = Vector(vertices[i], center);
                vertices[i] = (center + help_vetor).get_point();
            }
        }

        void reflex(Line axis) override {
            for (int i = 0; i < verticesCount(); ++i) {
                vertices[i] = axis.get_reflex(vertices[i]);
            }
        }

        void scale(Point center, double coefficient) override {
            for (int i = 0; i < verticesCount(); ++i) {
                vertices[i] = (Vector(center, vertices[i]) * coefficient).get_point();
            }
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

    class Ellipse : public Shape {
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
            Line main_axis(focus_a, focus_b);
            Point on_f_directr = (Vector(focus_a,focus_b) * ((A * A) / (C * C * 2))).get_point();
            Point on_s_directr = (Vector(focus_a,focus_b) * ((A * A) / (C * C * -2))).get_point();
            return std::make_pair(main_axis.get_perpendicular(on_f_directr), main_axis.get_perpendicular(on_s_directr));
        }

        double area() override {
            return M_PI * A * get_B();
        }

        double perimeter() override {
            double B = get_B();
            return M_PI * (3 * (A + B) - sqrt((3 * A + B) * (A + 3 * B)));
        }

        bool operator ==(const Shape& another) override {
            Ellipse an = dynamic_cast<const Ellipse&> (another);
            return focus_a == an.focus_a and
                   focus_b == an.focus_b and
                   isEqual(A , an.A);
        }

        bool isCongruentTo(const Shape& another) override {
            Ellipse an = dynamic_cast<const Ellipse&> (another);
            return isEqual(A, an.A) and isEqual(C, an.C);
        }

        bool isSimilarTo(const Shape& another) override {
            Ellipse an = dynamic_cast<const Ellipse&> (another);
            return isEqual(A/an.A, get_B()/an.get_B());
        }

        bool containsPoint(Point point) override {
            Point center = get_center();
            double B = get_B();
            return (pow((point.x - center.x) * B, 2) + pow((point.y - center.y) * A, 2)) <= A*A*B*B;
        }

        void rotate(Point center, double angle) override {
            Vector help_vector(center, focus_a);
            help_vector.rotate(angle * M_PI / 180);
            focus_a = (center + help_vector).get_point();

            help_vector = Vector(center, focus_b);
            help_vector.rotate(angle * M_PI / 180);
            focus_b = (center + help_vector).get_point();
        }

        void reflex(Point center) override  {
            Vector help_vector(focus_a, center);
            focus_a = (center + help_vector).get_point();

            help_vector = Vector(focus_b, center);
            focus_b = (center + help_vector).get_point();
        }

        void reflex(Line axis) override {
            focus_a = axis.get_reflex(focus_a);
            focus_b = axis.get_reflex(focus_b);
        }

        void scale(Point center, double coefficient) override {
            focus_a = (Vector(center, focus_a) * coefficient).get_point();
            focus_b = (Vector(center, focus_b) * coefficient).get_point();
        }


    private:
        Point focus_a;
        Point focus_b;
        double A;
        double C;

        double get_B() const {
            return sqrt(A * A - C * C);
        }

        Point get_center() const {
            return (focus_b + Vector(focus_b, focus_a) * 0.5).get_point();
        }
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

        void rotate(Point center, double angle) override {
            Vector help_vector(center, cent);
            help_vector.rotate(angle * M_PI / 180);
            cent = (center + help_vector).get_point();
        }

        void reflex(Point center) override {
            Vector help_vector(cent, center);
            cent = (center + help_vector).get_point();
        }

        void reflex(Line axis) override {
            cent = axis.get_reflex(cent);
        }

        void scale(Point center, double coefficient) override {
            cent = (Vector(center, cent) * coefficient).get_point();
        }

        friend std::ostream& operator <<(std::ostream& out, const Circle& circle);

    private:
        Point cent;
        double rad;

        void update_cent(){
            cent = focuses().first;
        }
    };

    std::ostream& operator <<(std::ostream& out, const Circle& circle){
        out << " center : " << circle.cent << std::endl;
        out << " radius : " << circle.radius() << std::endl;
        return out;
    }

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
            Line l1 = get_bisector(0);
            Line l2 = get_bisector(1);
            Point center = (l1 ^ l2);
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
            Vector a(vertices[vex], vertices[(vex + 1) % 3]);
            Vector b(vertices[vex], vertices[(vex + 2) % 3]);
            Vector guide_vect = (a + b*(a.get_vector_len()/b.get_vector_len()));
            Line bisector(vertices[vex], guide_vect);
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