#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <complex>

extern "C" bool is_in_mandelbrot(std::complex<double> c, int N);

#endif
