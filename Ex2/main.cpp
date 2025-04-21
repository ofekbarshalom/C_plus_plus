// ofekbarshalom@gmail.com

#include "SquareMat.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace MatrixSpace;

int main() {
    SquareMat a(2);
    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;

    SquareMat b(2);
    b[0][0] = 5; b[0][1] = 6;
    b[1][0] = 7; b[1][1] = 8;

    cout << "Matrix A:\n" << a << endl;
    cout << "Matrix B:\n" << b << endl;

    cout << "A + B:\n" << (a + b) << endl;
    cout << "A - B:\n" << (a - b) << endl;
    cout << "A * B:\n" << (a * b) << endl;
    cout << "A * 2.0:\n" << (a * 2.0) << endl;
    cout << "2.0 * A:\n" << (2.0 * a) << endl;
    cout << "-A:\n" << (-a) << endl;
    cout << "~A (Transpose):\n" << (~a) << endl;
    cout << "A % B (Element-wise multiplication):\n" << (a % b) << endl;
    cout << "A % 3 (Modulo scalar):\n" << (a % 3) << endl;
    cout << "A / 2.0:\n" << (a / 2.0) << endl;
    cout << "A ^ 2:\n" << (a ^ 2) << endl;

    cout << "Determinant of A (!A): " << !a << "\n" << endl;

    cout << "Comparisons:\n";
    cout << "A == B: " << (a == b) << "\n";
    cout << "A != B: " << (a != b) << "\n";
    cout << "A < B: " << (a < b) << "\n";
    cout << "A <= B: " << (a <= b) << "\n";
    cout << "A > B: " << (a > b) << "\n";
    cout << "A >= B: " << (a >= b) << "\n";

    return 0;
}
