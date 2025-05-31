#include <iostream>
#include <complex>
#include <iomanip>
#include <cstdlib>

using namespace std;

bool isInMandelbrot(const complex<double>& c, int N = 1000) {
    complex<double> z = 0;
    for (int i = 0; i < N; ++i) {
        z = z * z + c;
        if (abs(z) > 2.0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4 || string(argv[1]) == "--help") {
        cerr << "Usage: " << argv[0] << " <real> <imag> [N]" << endl;
        cerr << "Example: " << argv[0] << " -1.5 0.01 1000" << endl;
        return 1;
    }

    double real = atof(argv[1]);
    double imag = atof(argv[2]);
    int N = 1000;  // default value

    if (argc == 4) {
        N = atoi(argv[3]);
    }

    complex<double> c(real, imag);

    cout << fixed << setprecision(3);
    if (!isInMandelbrot(c, N)) {
        cout << "The number " << c << " is not in the Mandelbrot set!" << endl;
    } else {
        cout << "The number " << c << " is in the Mandelbrot set!" << endl;
    }

    return 0;
}
