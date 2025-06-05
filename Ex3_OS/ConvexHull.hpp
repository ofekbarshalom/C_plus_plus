#ifndef CONVEX_HULL_HPP
#define CONVEX_HULL_HPP

#include <vector>

using namespace std;

// Represents a 2D point
struct Point {
    double x, y;
};

double cross(const Point& a, const Point& b, const Point& c);
vector<Point> ConvexHull(vector<Point> points);
double polygonArea(const vector<Point>& poly);

#endif
