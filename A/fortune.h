#ifndef FORTUNE_H
#define FORTUNE_H

#include <bits/stdc++.h>
#include "point.h"
#include "arc.h"
#include "event.h"

using namespace std;

long double sweepX;

struct Fortune 
{
    multiset<Arc,less<>> beachLine;
    vector<pair<Point, int>> points;
    priority_queue<Event> eventQueue;
    vector<pair<int, int>> edges;  
    vector<bool> validEvents;
    int n, eventCounter;
    map<int,Point> mp;
    Fortune(vector<Point> &pts) 
    {
        n = pts.size();
        points.resize(n);
        for (int i = 0; i < n; i++) {
            points[i] = {pts[i], i};
            mp[i]=pts[i];
        }
        sort(points.begin(), points.end(), [](auto &a, auto &b) { return a.first.x < b.first.x; });
    }

    void addCircleEvent(multiset<Arc>::iterator it) 
    {
        if (it->index == -1) return;
        validEvents[-it->id] = false;
        auto prevArc = prev(it);
        if (collinear(it->q - it->p, prevArc->p - it->p)) return;
        it->id = --eventCounter;
        validEvents.push_back(true);
        Point c = circumcenter(it->p, it->q, prevArc->p);
        long double x = c.x + (c - it->p).length();
        if (x > sweepX - EPS && prevArc->getY(x) + EPS > it->getY(x)) 
        {
            eventQueue.push(Event(x, it->id, it));
        }
    }

    void addEdge(int i, int j) 
    {
        if (i == -1 || j == -1) return;
        edges.emplace_back(points[i].second, points[j].second);
    }

    void handlePoint(int index) 
    {
        Point p = points[index].first;
        auto arc = beachLine.lower_bound(p.y);
        auto left = beachLine.insert(arc, Arc(p, arc->p, index));
        auto right = beachLine.insert(left, Arc(arc->p, p, arc->index));
        addEdge(index, arc->index);
        addCircleEvent(left);
        addCircleEvent(right);
        addCircleEvent(arc);
    }

    void handleVertex(multiset<Arc>::iterator it) 
    {
        auto left = prev(it);
        auto right = next(it);
        //Point c = circumcenter(left->p, it->p, right->p);
        beachLine.erase(it);
        left->q = right->p;
        addEdge(left->index, right->index);
        addCircleEvent(left);
        addCircleEvent(right);
    }

    void solve(long double boundary = 1e9) 
    {
        boundary *= 3;
        beachLine.insert(Arc(Point(-boundary, -boundary), Point(-boundary, boundary), -1));
        beachLine.insert(Arc(Point(-boundary, boundary), Point(INF, INF), -1));
        
        for (int i = 0; i < n; i++) 
            eventQueue.push(Event(points[i].first.x, i, beachLine.end()));
        
        eventCounter = 0;
        validEvents.assign(1, false);
        
        while (!eventQueue.empty()) {
            Event e = eventQueue.top();
            eventQueue.pop();
            sweepX = e.x;
            
            if (e.id >= 0) {
                handlePoint(e.id);
            } else if (validEvents[-e.id]) {
                handleVertex(e.it);
            }
        }
    }
};

#endif // FORTUNE_H
