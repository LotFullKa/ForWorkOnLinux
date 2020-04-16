struct Point{
public:
    Point(double x, double y) : x(x), y(y){}

    double x;
    double y;

    bool operator ==(const Point &other) const {
        return (this->x == other.x and this->y == other.y);
    }

    bool operator !=(const Point &other) const {
        return (this->x != other.x or this->y != other.y);
    }
};
