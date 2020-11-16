#ifndef XG_GRAPHICS_UTIL_POINT_H
#define XG_GRAPHICS_UTIL_POINT_H

#include <math.h>

namespace xg {
namespace util {

class Point {
    public:
    double x;
    double y;
    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}
    Point(const Point &p) : x(p.x), y(p.y) {}

    void Reset(double _x, double _y) {
        x = _x;
        y = _y;
    }
    bool operator==(Point &&other) { return this->x == other.x && this->y == other.y; }

    static Point Max(Point &p1, Point &p2) {
        Point out = Point(fmax(p1.x, p2.x), fmax(p1.y, p2.y));
        return out;
    }

    static Point Min(Point &p1, Point &p2) {
        Point out = Point(fmin(p1.x, p2.x), fmin(p1.y, p2.y));
        return out;
    }

    static Point Add(Point &p1, Point &p2) {
        Point out = Point(p1.x + p2.x, p1.y + p2.y);
        return out;
    }

    static Point Sub(Point &p1, Point &p2) {
        Point out = Point(p1.x - p2.x, p1.y - p2.y);
        return out;
    }

    static Point Scale(Point &p, double b) {
        Point out = Point(p.x * b, p.y * b);
        return out;
    }

    static double Distance(Point &p1, Point &p2) {
        double x = p2.x - p1.x;
        double y = p2.y - p1.y;
        return hypot(x, y);
    }
};
class BezierPoint {
    public:
    Point cp1;
    Point cp2;
    Point p;

    BezierPoint(const Point &_cp1, const Point &_cp2, const Point &_p) : cp1(Point(_cp1)), cp2(Point(_cp2)), p(Point(_p)) {}
};

class Size {
    public:
    double width;
    double height;

    Size() : width(0), height(0) {}
    Size(double _w, double _h) : width(_w), height(_h) {}
    Size(const Size &p) : width(p.width), height(p.height) {}

    void Reset(double _w, double _h) {
        width = _w;
        height = _h;
    }
};

class Rect {
    public:
    double x;
    double y;
    double width;
    double height;
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(double _x, double _y, double _width, double _height) : x(_x), y(_y), width(_width), height(_height) {}
    Rect(const Rect &r) : x(r.x), y(r.y), width(r.width), height(r.height) {}

    void Reset(double _x, double _y, double _width, double _height) {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }
};
} // namespace util
} // namespace xg
#endif // XG_GRAPHICS_UTIL_POINT_H
