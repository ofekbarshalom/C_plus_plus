#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "ConvexHull.hpp"

using namespace std;

// Cross product of vectors (b - a) x (c - a)
double cross(const Point& a, const Point& b, const Point& c){
    return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

// Convex hull using Andrew's Monotone Chain algorithm
vector<Point> ConvexHull(vector<Point> points) {
    int n = points.size();
    if( n <= 1) return points;

    // Sort by x, then y
    sort(points.begin(), points.end(), [](const Point& a, const Point& b){
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    vector<Point> hull;

    // Lower part
    for (const Point& p : points) {
        while(hull.size() >= 2 && cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0){
            hull.pop_back();
        }
        hull.push_back(p);
    }

    // Upper part
    size_t lowerSize = hull.size();

    for(int i = n - 2; i >= 0; --i) {
        while(hull.size() > lowerSize && cross(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0){
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // Remove duplicate last point
    if (!hull.empty()){
        hull.pop_back();
    }

    return hull;
}

// Compute the convex hull area
double polygonArea(const vector<Point>& poly) {
    double area = 0.0;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        const Point& p1 = poly[i];
        const Point& p2 = poly[(i + 1) % n]; // wrap around to the first point
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return abs(area) / 2.0;
}