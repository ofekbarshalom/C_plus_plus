#include "mandelbrot.h"
#include <iostream>
#include <complex>
#include <iomanip>

using namespace std;

int main() {
    double real, imag;
    int N;

    cout << fixed << setprecision(3);

    while (true) {
        cout << "\nEnter the real and imag (0 0 to exit): ";
        if (!(cin >> real >> imag)) {
            cerr << "Invalid input!" << endl;
            return 1;
        }

        // Exit condition
        if (real == 0.0 && imag == 0.0) {
            cout << "Exiting.\n";
            break;
        }

        cout << "Enter N: ";
        if (!(cin >> N)) {
            cerr << "Invalid input!" << endl;
            return 1;
        }

        complex<double> c(real, imag);

        if (is_in_mandelbrot(c, N)) {
            cout << "The number " << c << " is in the Mandelbrot set." << endl;
        } else {
            cout << "The number " << c << " is NOT in the Mandelbrot set." << endl;
        }
    }

    return 0;
}
