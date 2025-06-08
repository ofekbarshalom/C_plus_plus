#ifndef CONVEX_HULL_VECTOR_HPP
#define CONVEX_HULL_VECTOR_HPP

#include <vector>
#include "Point.hpp"

using namespace std;

namespace vectorConvexHull{
    double cross(const Point& a, const Point& b, const Point& c);
    vector<Point> ConvexHull(vector<Point> points);
    double polygonArea(const vector<Point>& poly);
}

#endif
