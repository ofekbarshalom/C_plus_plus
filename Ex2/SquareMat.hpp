// ofekbarshalom@gmail.com

#ifndef SQUAREMAT_HPP
#define SQUAREMAT_HPP

#include <iostream>
#include <stdexcept>
#include <cstddef>

namespace MatrixSpace {

class SquareMat {
private:
    size_t size;
    double* data;

public:
    // Constructors
    SquareMat(size_t n); // Constructor
    SquareMat(const SquareMat& mat); // Copy constructor
    SquareMat& operator=(const SquareMat& mat); // Copy assignment operator
    ~SquareMat(); // Destructor

    // Arithmetic Operators
    SquareMat operator+(const SquareMat& other) const;
    SquareMat operator-(const SquareMat& other) const;
    SquareMat operator-() const;
    SquareMat operator*(const SquareMat& other) const;
    SquareMat operator*(double scalar) const;
    SquareMat operator%(const SquareMat& other) const;
    SquareMat operator%(int scalar) const;
    SquareMat operator/(double scalar) const;
    SquareMat operator^(int power) const;

    // compound Assignment operators
    SquareMat& operator+=(const SquareMat& other);
    SquareMat& operator-=(const SquareMat& other);
    SquareMat& operator*=(const SquareMat& other);
    SquareMat& operator*=(double scalar);
    SquareMat& operator%=(const SquareMat& other);
    SquareMat& operator%=(int scalar);
    SquareMat& operator/=(double scalar);

    // Increment / Decrement operators
    SquareMat& operator++(); // Pre-increment
    SquareMat operator++(int); // Post-increment
    SquareMat& operator--(); // Pre-Decrement
    SquareMat operator--(int); // Post-Decrement

    // Transpose operator
    SquareMat operator~() const;

    // Helper function
    double sum() const;

    // Comparison operator
    bool operator==(const SquareMat& other) const;
    bool operator!=(const SquareMat& other) const;
    bool operator<(const SquareMat& other) const;
    bool operator>(const SquareMat& other) const;
    bool operator<=(const SquareMat& other) const;
    bool operator>=(const SquareMat& other) const;

    // Helper function
    SquareMat getMinor(size_t row, size_t col) const;

    // Determinant operator
    double operator!() const;

    // Element access
    double* operator[](size_t row);
    const double* operator[](size_t row) const;

    // Friend functions
    friend SquareMat operator*(double scalar, const SquareMat& mat);
    friend std::ostream& operator<<(std::ostream& os, const SquareMat& mat);
};

} // namespace MatrixSpace



#endif //SQUAREMAT_HPP
