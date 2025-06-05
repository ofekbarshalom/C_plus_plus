#ifndef CONVEX_HULL_DEQUE_HPP
#define CONVEX_HULL_DEQUE_HPP

#include <deque>
#include "Point.hpp"

using namespace std;

namespace dequeConvexHull{
    double cross(const Point& a, const Point& b, const Point& c);
    deque<Point> ConvexHull(deque<Point> points);
    double polygonArea(const deque<Point>& poly);
}

#endif
