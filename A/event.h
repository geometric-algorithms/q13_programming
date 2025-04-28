#ifndef EVENT_H
#define EVENT_H

#include "arc.h"
#include <queue>
#include <set>

struct Event 
{
    long double x;
    int id;
    std::multiset<Arc>::iterator it;
    
    Event(long double x, int id, std::multiset<Arc>::iterator it) 
        : x(x), id(id), it(it) {}

    bool operator<(const Event &e) const { return x > e.x; }
};

#endif // EVENT_H
