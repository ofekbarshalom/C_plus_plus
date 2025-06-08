#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "ConvexHullVector.hpp"

using namespace std;
using namespace vectorConvexHull;

vector<Point> points;

// Parses a string into two doubles
void parsePoint(const string& str, double& x, double& y) {
    string s = str;
    replace(s.begin(), s.end(), ',', ' ');
    stringstream ss(s);
    ss >> x >> y;
}

// Handle: Newgraph n
void handleNewgraph(int n) {
    points.clear();
    for (int i = 0; i < n; ++i) {
        string line;
        getline(cin, line);
        double x, y;
        parsePoint(line, x, y);
        points.push_back({x, y});
    }
}

// Handle: Newpoint x,y
void handleNewpoint(const string& args) {
    double x, y;
    parsePoint(args, x, y);
    points.push_back({x, y});
}

// Handle: Removepoint x,y
void handleRemovepoint(const string& args) {
    double x, y;
    parsePoint(args, x, y);
    auto it = remove_if(points.begin(), points.end(), [&](const Point& p) {
        return p.x == x && p.y == y;
    });
    points.erase(it, points.end());
}

// Handle: CH
void handleCH() {
    vector<Point> hull = ConvexHull(points);
    double area = polygonArea(hull);
    cout << fixed << setprecision(4) << area << endl;
}

int main() {
    string line;
    while (getline(cin, line)) {
        if (line.rfind("Newgraph", 0) == 0) {
            int n = stoi(line.substr(9));
            handleNewgraph(n);
        } else if (line == "CH") {
            handleCH();
        } else if (line.rfind("Newpoint", 0) == 0) {
            handleNewpoint(line.substr(9));
        } else if (line.rfind("Removepoint", 0) == 0) {
            handleRemovepoint(line.substr(12));
        } else {
            cerr << "Unknown command: " << line << endl;
        }
    }

    return 0;
}
