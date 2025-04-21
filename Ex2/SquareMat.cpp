// ofekbarshalom@gmail.com

#include "SquareMat.hpp"
#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

namespace MatrixSpace {

    // Constructor: creates an n*n matrix filled with zeros
    SquareMat::SquareMat(size_t n) : size(n) {
        if (n == 0) {
            throw invalid_argument("Matrix size must be greater than 0");
        }
        data = new double[size*size];
        memset(data, 0, size*size*sizeof(double)); // initialize to 0
    }

    // Copy constructor
    SquareMat::SquareMat(const SquareMat& other) : size(other.size) {
        data = new double[size*size];
        memcpy(data, other.data, size*size*sizeof(double));
    }

    // Copy assigment operator
    SquareMat& SquareMat::operator=(const SquareMat& other) {
        if (this == &other) return *this;

        delete[] data;
        size = other.size;
        data = new double[size*size];
        memcpy(data, other.data, size*size*sizeof(double));
        return *this;
    }

    //Destructor
    SquareMat::~SquareMat() {
        delete[] data;
    }

    // Arithmetic Operators

    //Addition operator
    SquareMat SquareMat::operator+(const SquareMat& other) const {
        if (size != other.size) {
            throw invalid_argument("Matrix sizes must match for addition");
        }

        SquareMat result(size);
        for (size_t i = 0; i < size * size; ++i) {
            result.data[i] = this->data[i] + other.data[i];
        }
        return result;
    }

    // Subtraction operator
    SquareMat SquareMat::operator-(const SquareMat& other) const {
        if (size != other.size) {
            throw invalid_argument("Matrix sizes must match for subtraction");
        }

        SquareMat result(size);
        for (size_t i = 0; i < size * size; ++i) {
            result.data[i] = this->data[i] - other.data[i];
        }
        return result;
    }

    // Unary minus
    SquareMat SquareMat::operator-() const {
        SquareMat result(size);

        for (size_t i = 0; i < size*size; ++i) {
            result.data[i] = -data[i];
        }

        return result;
    }

    //  Multiplication operator
    SquareMat SquareMat::operator*(const SquareMat& other) const {
        if (size != other.size) {
            throw invalid_argument("Matrix sizes must match for multiplication");
        }

        SquareMat result(size);

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                double sum = 0;
                for (size_t k = 0; k < size; ++k) {
                    sum += data[i * size + k] * other.data[k * size + j];
                }
                result.data[i * size + j] = sum;
            }
        }

        return result;
    }

    // Scalar multiplication operator
    SquareMat SquareMat::operator*(double scalar) const {
        SquareMat result(size);

        for (size_t i = 0; i < size * size; ++i) {
            result.data[i] = data[i] * scalar;
        }

        return result;
    }

    // % multiplication operator
    SquareMat SquareMat::operator%(const SquareMat& other) const {
        if (size != other.size) {
            throw invalid_argument("Matrix sizes must match for special multiplication");
        }

        SquareMat result(size);
        for (size_t i = 0; i < size * size; ++i) {
            result.data[i] = this->data[i] * other.data[i];
        }
        return result;
    }

    // module operator
    SquareMat SquareMat::operator%(int scalar) const {
        if (scalar == 0) {
            throw invalid_argument("Cannot modulo by zero");
        }

        SquareMat result(size);

        for (size_t i = 0; i < size*size; ++i) {
            result.data[i] = fmod(data[i], scalar);
        }

        return result;
    }

    // Divide by scalar operator
    SquareMat SquareMat::operator/(double scalar) const {
        if (scalar == 0.0) {
            throw invalid_argument("Division by zero is not allowed");
        }

        SquareMat result(size);

        for (size_t i = 0; i < size*size; ++i) {
            result.data[i] = data[i] / scalar;
        }

        return result;
    }

    // pow operator
    SquareMat SquareMat::operator^(int power) const {
        if (power < 0) {
            throw invalid_argument("Negative powers are not supported");
        }
        if (power == 0) {
            SquareMat identity(size);
            for (size_t i = 0; i < size; ++i) {
                identity[i][i] = 1;
            }
            return identity;
        }

        SquareMat result(*this);

        for (int i = 1; i < power; ++i) {
            result = result * SquareMat(*this);
        }

        return result;
    }

    // Adds another matrix to this matrix
    SquareMat& SquareMat::operator+=(const SquareMat& other) {
        if (size != other.size) {
            throw invalid_argument("Matrix sizes must match for addition");
        }

        for (size_t i = 0; i < size * size; ++i) {
            data[i] += other.data[i];
        }

        return *this;
    }

    // Subtracts another matrix from this matrix
    SquareMat& SquareMat::operator-=(const SquareMat& other) {
        if (size != other.size) {
            throw invalid_argument("Matrix sizes must match for subtraction");
        }

        for (size_t i = 0; i < size * size; ++i) {
            data[i] -= other.data[i];
        }

        return *this;
    }

    // Multiplies this matrix by another matrix.
    SquareMat& SquareMat::operator*=(const SquareMat& other) {
        if (size != other.size) {
            throw std::invalid_argument("Matrix sizes must match for multiplication");
        }

        SquareMat temp(*this); // read from this

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                double sum = 0;
                for (size_t k = 0; k < size; ++k) {
                    sum += temp[i][k] * other[k][j];
                }
                (*this)[i][j] = sum;
            }
        }

        return *this;
    }


    // Multiplies this matrix by a scalar.
    SquareMat& SquareMat::operator*=(double scalar) {
        for (size_t i = 0; i < size * size; ++i) {
            data[i] *= scalar;
        }
        return *this;
    }

    // Performs element-wise multiplication with another matrix.
    SquareMat& SquareMat::operator%=(const SquareMat& other) {
        if (size != other.size) {
            throw invalid_argument("Matrix sizes must match for element-wise multiplication");
        }

        for (size_t i = 0; i < size * size; ++i) {
            data[i] *= other.data[i];
        }

        return *this;
    }

    // Performs modulo with a scalar on each element.
    SquareMat& SquareMat::operator%=(int scalar) {
        if (scalar == 0) {
            throw invalid_argument("Cannot modulo by zero");
        }

        for (size_t i = 0; i < size * size; ++i) {
            data[i] = fmod(data[i], scalar);
        }

        return *this;
    }

    // Divides this matrix by a scalar.
    SquareMat& SquareMat::operator/=(double scalar) {
        if (scalar == 0.0) {
            throw invalid_argument("Division by zero is not allowed");
        }

        for (size_t i = 0; i < size * size; ++i) {
            data[i] /= scalar;
        }

        return *this;
    }

    // Increment / Decrement operators

    // Pre-increment
    SquareMat& SquareMat::operator++() {
        for (size_t i = 0; i < size * size; ++i) {
            data[i]++;
        }
        return *this;
    }

    // Post-increment
    SquareMat SquareMat::operator++(int) {
        SquareMat temp(*this);
        ++(*this);
        return temp;
    }

    // Pre-Decrement
    SquareMat& SquareMat::operator--() {
        for (size_t i = 0; i < size * size; ++i) {
            data[i]--;
        }
        return *this;
    }

    // Post-Decrement
    SquareMat SquareMat::operator--(int) {
        SquareMat temp(*this);
        --(*this);
        return temp;
    }

    // Transpose operator
    SquareMat SquareMat::operator~() const {
        SquareMat result(size);

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.data[i * size + j] = this->data[j * size + i];
            }
        }
        return result;
    }

    // Comparison operator

    // Helper function that calculate the sum of the matrix
    double SquareMat::sum() const {
        double total = 0;
        for (size_t i = 0; i < size * size; ++i) {
            total += data[i];
        }
        return total;
    }

    // Returns true if the sum of elements is equal.
    bool SquareMat::operator==(const SquareMat& other) const {
        return sum() == other.sum();
    }

    // Returns true if the sum of elements is not equal.
    bool SquareMat::operator!=(const SquareMat& other) const {
        return sum() != other.sum();
    }

    // Returns true if this matrix's sum is less than the other's.
    bool SquareMat::operator<(const SquareMat& other) const {
        return sum() < other.sum();
    }

    // Returns true if this matrix's sum is greater than the other's.
    bool SquareMat::operator>(const SquareMat& other) const {
        return sum() > other.sum();
    }

    // Returns true if this matrix's sum is less than or equal to the other's.
    bool SquareMat::operator<=(const SquareMat& other) const {
        return sum() <= other.sum();
    }

    // Returns true if this matrix's sum is greater than or equal to the other's.
    bool SquareMat::operator>=(const SquareMat& other) const {
        return sum() >= other.sum();
    }

    // Helper function that returns the minor matrix after removing row 'row' and column 'col'.
    SquareMat SquareMat::getMinor(size_t row, size_t col) const {
        SquareMat minor(size - 1);
        size_t dst = 0;

        for (size_t i = 0; i < size; ++i) {
            if (i == row) continue;
            for (size_t j = 0; j < size; ++j) {
                if (j == col) continue;
                minor.data[dst++] = data[i * size + j];
            }
        }

        return minor;
    }

    // Determinant operator
    double SquareMat::operator!() const {
        if (size == 1) return data[0];

        if (size == 2) {
            return data[0] * data[3] - data[1] * data[2];
        }

        double det = 0.0;
        for (size_t j = 0; j < size; ++j) {
            SquareMat minor = getMinor(0,j);
            double cofactor = ((j % 2 == 0) ? 1 : -1) * data[j] * !minor;
            det += cofactor;
        }
        return det;
    }

    // Element access

    // Returns a pointer to the beginning of row 'row' (non-const version)
    double* SquareMat::operator[](size_t row) {
        if (row >= size) {
            throw out_of_range("Matrix index out of range");
        }
        return &data[row * size];
    }

    // Returns a pointer to the beginning of row 'row' (const version)
    const double* SquareMat::operator[](size_t row) const {
        if (row >= size) {
            throw out_of_range("Matrix index out of range");
        }
        return &data[row * size];
    }

    // Friend functions

    // Scalar multiplication operator
    SquareMat operator*(double scalar, const SquareMat &mat) {
        return  mat * scalar;
    }

    // Prints the matrix in a readable format
    ostream& operator<<(ostream& os, const SquareMat& mat) {
        for (size_t i = 0; i < mat.size; ++i) {
            os << "[ ";
            for (size_t j = 0; j < mat.size; ++j) {
                os << mat.data[i * mat.size + j];
                if (j < mat.size - 1) os << ", ";
            }
            os << " ]\n";
        }
        return os;
    }
}
