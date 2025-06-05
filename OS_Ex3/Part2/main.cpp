#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>

#include "ConvexHullVector.hpp"  // vectorConvexHull::ConvexHull
#include "ConvexHullDeque.hpp"   // ConvexHull, polygonArea with deque

using namespace std;
using namespace std::chrono;

int main() {
    int n;
    cin >> n;
    cin.ignore();  // To ignore the newline after the number

    vector<Point> vecPoints;

    // Read n lines like: 1.0,2.0
    for (int i = 0; i < n; ++i) {
        string line;
        getline(cin, line);

        // Replace comma with space to allow easy parsing
        replace(line.begin(), line.end(), ',', ' ');

        stringstream ss(line);
        double x, y;
        ss >> x >> y;

        vecPoints.push_back({x, y});
    }

    // Copy same input to deque
    deque<Point> deqPoints(vecPoints.begin(), vecPoints.end());

    // ---------- Vector implementation ----------
    auto startVec = high_resolution_clock::now();
    vector<Point> vecHull = vectorConvexHull::ConvexHull(vecPoints);
    double vecArea = vectorConvexHull::polygonArea(vecHull);
    auto endVec = high_resolution_clock::now();
    auto vecTime = duration_cast<microseconds>(endVec - startVec).count();

    // ---------- Deque implementation ----------
    auto startDeq = high_resolution_clock::now();
    deque<Point> deqHull = dequeConvexHull::ConvexHull(deqPoints);
    double deqArea = dequeConvexHull::polygonArea(deqHull);
    auto endDeq = high_resolution_clock::now();
    auto deqTime = duration_cast<microseconds>(endDeq - startDeq).count();

    // ---------- Output Results ----------
    cout << fixed << setprecision(4);
    cout << "== Vector Version ==" << endl;
    cout << "Area: " << vecArea << ", Time: " << vecTime << " microseconds" << endl;

    cout << "\n== Deque Version ==" << endl;
    cout << "Area: " << deqArea << ", Time: " << deqTime << " microseconds" << endl;

    // ---------- Summary ----------
    cout << "\n== Summary ==" << endl;
    if (vecTime < deqTime)
        cout << "Vector version is faster by " << deqTime - vecTime << " microseconds." << endl;
    else if (deqTime < vecTime)
        cout << "Deque version is faster by " << vecTime - deqTime << " microseconds." << endl;
    else
        cout << "Both versions took the same time." << endl;

    return 0;
}
