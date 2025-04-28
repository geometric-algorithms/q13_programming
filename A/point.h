#ifndef POINT_H
#define POINT_H

#include <cmath>

const long double EPS = 1e-12;
const long double INF = 1e100;

struct Point {
    long double x, y;
    Point(long double x = 0, long double y = 0) : x(x), y(y) {}

    Point operator+(const Point &p) const { return {x + p.x, y + p.y}; }
    Point operator-(const Point &p) const { return {x - p.x, y - p.y}; }
    Point operator*(long double f) const { return {x * f, y * f}; }
    Point rotate() const { return {-y, x}; }
    long double length() const { return hypotl(x, y); }
};

inline long double cross(Point a, Point b) 
{
    return a.x * b.y - a.y * b.x;
}
inline long double dot(Point a, Point b) 
{
    return a.x * b.x + a.y * b.y;
}
inline Point reflect(Point p, Point a, Point b) 
{
    Point proj = a + (b - a) * (dot(p - a, b - a) / dot(b - a, b - a));
    return proj * 2 - p;
}
inline bool collinear(Point a, Point b)
{
    return std::abs(cross(a,b)) < EPS;
}

inline Point lineline(Point a, Point b, Point c, Point d) 
{
    return a + (b - a) * (cross(c-a,d-c) / cross(b-a,d-c));
}

inline Point circumcenter(Point a, Point b, Point c) 
{
    b = (a + b) * 0.5;
    c = (a + c) * 0.5;
    return lineline(b, b + (b - a).rotate(), c, c + (c - a).rotate());
}

inline long double squaredDistance(Point a, Point b) 
{
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline int getTriangleType(Point A, Point B, Point C) 
{
    double a2 = squaredDistance(B, C);
    double b2 = squaredDistance(A, C);
    double c2 = squaredDistance(A, B);

    if (a2 + b2 > c2 && a2 + c2 > b2 && b2 + c2 > a2)
        return 0;  
    else if (a2 > b2 && a2 > c2)
        return 1;  
    else if (b2 > a2 && b2 > c2)
        return 2;  
    else
        return 3; 
}

inline int pointSide(Point A, Point B, Point C) 
{
    long double val = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
    if (val > 0) return 1;  
    if (val < 0) return -1;
    return 0;  
}

inline long double perpendicularDistance(Point A, Point B, Point C) {
    long double num = abs((B.y - A.y) * C.x - (B.x - A.x) * C.y + B.x * A.y - B.y * A.x);
    long double den = sqrt(squaredDistance(A, B));
    return num / den;
}
#endif // POINT_H
