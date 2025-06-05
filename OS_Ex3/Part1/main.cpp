#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "ConvexHull.hpp"

int main() {
    int n;
    cin >> n;
    cin.ignore();

    vector<Point> points;

    for (int i = 0; i < n; ++i) {
        string line;
        getline(cin, line);

        replace(line.begin(), line.end(), ',', ' ');

        stringstream ss(line);
        double x,y;
        ss >> x >> y;

        points.push_back({x, y});
    }

    vector<Point> hull = ConvexHull(points);

    double area = polygonArea(hull);
    cout << area << endl;

    return 0;
}