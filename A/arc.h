#ifndef ARC_H
#define ARC_H

#include "point.h"
#include <set>

extern long double sweepX;

struct Arc 
{
    mutable Point p, q;
    mutable int id = 0, index;
    Arc(Point p, Point q, int index) : p(p), q(q), index(index) {}

    long double getY(long double x) const 
    {
        if (q.y == INF) return INF;
        x += EPS;
        Point mid = (p + q) * 0.5;
        Point dir = (p - mid).rotate();
        long double D = (x - p.x) * (x - q.x);
        return mid.y + ((mid.x - x) * dir.x + sqrtl(D) * dir.length()) / dir.y;
    }
    bool operator<(const long double &y) const { return getY(sweepX) < y; }
    bool operator<(const Arc &o) const { return getY(sweepX) < o.getY(sweepX); }
};

#endif // ARC_H
