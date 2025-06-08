#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "ConvexHull.hpp"

int main() {
    int n;
    cin >> n;
    cin.ignore();

    // Open input file
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error: Cannot open input.txt" << endl;
        return 1;
    }

    vector<Point> points;

    for (int i = 0; i < n; ++i) {
        string line;
        getline(inputFile, line);

        // Replace comma with space
        replace(line.begin(), line.end(), ',', ' ');

        stringstream ss(line);
        double x, y;
        ss >> x >> y;

        points.push_back({x, y});
    }

    inputFile.close();

    vector<Point> hull = ConvexHull(points);

    double area = polygonArea(hull);
    cout << area << endl;

    return 0;
}