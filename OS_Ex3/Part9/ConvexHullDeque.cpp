#include "ConvexHullDeque.hpp"
#include <algorithm>
#include <cmath>
#include "Point.hpp"

using namespace std;

namespace dequeConvexHull{

// Cross product
double cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

// Andrew's algorithm with deque
deque<Point> ConvexHull(deque<Point> points) {
    int n = points.size();
    if (n <= 1) return points;

    sort(points.begin(), points.end(), [](const Point& a, const Point& b){
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    deque<Point> hull;

    // Lower hull
    for (const Point& p : points) {
        while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }

    // Upper hull
    size_t lowerSize = hull.size();
    for (int i = n - 2; i >= 0; --i) {
        while (hull.size() > lowerSize && cross(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // Remove duplicate
    if (!hull.empty()) {
        hull.pop_back();
    }

    return hull;
}

// Area with deque
double polygonArea(const deque<Point>& poly) {
    double area = 0.0;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        const Point& p1 = poly[i];
        const Point& p2 = poly[(i + 1) % n];
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return abs(area) / 2.0;
}

}