#ifndef UTIL_H
#define UTIL_H

#include <string>
#include "SimpleImage.h"

using std::string;

#define PI 3.1415926535897932384626433832795028841968

inline float degreesToRad(float degree){
    return (float)(degree*PI/180.0);
}

inline int sign(float f) { return f >= 0 ? 1 : -1; }

// a 2D Point
struct Point2f {
    float x, y;

    Point2f(){ x = 0. ;  y = 0. ; }
    Point2f(float _x, float _y) { x = _x; y = _y; }

    Point2f operator*(float s) { return Point2f(x*s, y*s); }
    Point2f operator-() { return Point2f(-x, -y); }
    Point2f operator-(Point2f const o) { return Point2f(x-o.x, y-o.y); }
    Point2f operator+(Point2f const o) { return Point2f(x+o.x, y+o.y); }

};

// a 2D line, consisting in two points.
struct Line2f {
    Point2f a, b, s, m, r;

    Line2f(){ a = Point2f(); b = Point2f(); s = Point2f(); m = Point2f(); r = Point2f(); }
    Line2f(Point2f _a, Point2f _b, Point2f _s, Point2f _m, Point2f _r) { a = _a; b = _b; s= _s; m= _m; r= _r; }
};

// a planar triangle, consisting in three points
struct Triangle2f{
    Point2f a, b, c, s, m, r;
    RGBColor a_color, b_color, c_color;

    Triangle2f() {a = Point2f(); b = Point2f(); c = Point2f(); s = Point2f(); m = Point2f(); r = Point2f(); }
    Triangle2f(Point2f _a, Point2f _b, Point2f _c, Point2f _s, Point2f _m, Point2f _r) { a = _a; b = _b; c = _c; s= _s; m= _m; r= _r; }
    void color_verts(RGBColor _a, RGBColor _b, RGBColor _c) { a_color = _a; b_color = _b; c_color = _c;}
};

inline
double random_double(double min, double max){
    double range = max - min;
    return min + (double(rand()) / double(RAND_MAX)) * range;
}

inline
float random_float(float min, float max){
    float range = max - min;
    return min + (float(rand()) / float(RAND_MAX)) * range;
}

inline
int random_int(int min, int max)
{
    int range = max - min;
    return min + int((double(rand())/double(RAND_MAX)) * range);
}

inline
RGBColor random_rgbcolor(float scale)
{
    float r = random_float(0.f, scale);
    float g = random_float(0.f, scale);
    float b = random_float(0.f, scale);
    return RGBColor(r,g,b);
}
#endif
