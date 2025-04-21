// ofekbarshalom@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "SquareMat.hpp"

using namespace MatrixSpace;
using namespace std;

TEST_CASE("SquareMat test") {
    // Initialize two 2x2 matrices with predefined values
    SquareMat a(2);
    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;

    SquareMat b(2);
    b[0][0] = 5; b[0][1] = 6;
    b[1][0] = 7; b[1][1] = 8;

    // Test basic arithmetic operators

    SUBCASE("Addition operator") {
        SquareMat c = a + b;
        CHECK(c[0][0] == 6);
        CHECK(c[0][1] == 8);
        CHECK(c[1][0] == 10);
        CHECK(c[1][1] == 12);
    }

    SUBCASE("Subtraction operator") {
        SquareMat c = b - a;
        CHECK(c[0][0] == 4);
        CHECK(c[0][1] == 4);
        CHECK(c[1][0] == 4);
        CHECK(c[1][1] == 4);
    }

    SUBCASE("Unary minus operator") {
        SquareMat c = -a;
        CHECK(c[0][0] == -1);
        CHECK(c[0][1] == -2);
        CHECK(c[1][0] == -3);
        CHECK(c[1][1] == -4);
    }

    SUBCASE("Matrix multiplication operator") {
        SquareMat c = a * b;
        CHECK(c[0][0] == 19);
        CHECK(c[0][1] == 22);
        CHECK(c[1][0] == 43);
        CHECK(c[1][1] == 50);
    }

    SUBCASE("Scalar multiplication operator") {
        SquareMat c = a * 2.0;
        CHECK(c[0][0] == 2);
        CHECK(c[0][1] == 4);
        CHECK(c[1][0] == 6);
        CHECK(c[1][1] == 8);
    }

    SUBCASE("Element-wise multiplication operator (%)") {
        SquareMat c = a % b;
        CHECK(c[0][0] == 5);
        CHECK(c[0][1] == 12);
        CHECK(c[1][0] == 21);
        CHECK(c[1][1] == 32);
    }

    SUBCASE("Modulo with scalar operator (%)") {
        SquareMat c = a % 2;
        CHECK(c[0][0] == 1);
        CHECK(c[0][1] == 0);
        CHECK(c[1][0] == 1);
        CHECK(c[1][1] == 0);
    }

    SUBCASE("Division by scalar operator") {
        SquareMat c = b / 2.0;
        CHECK(c[0][0] == 2.5);
        CHECK(c[0][1] == 3.0);
        CHECK(c[1][0] == 3.5);
        CHECK(c[1][1] == 4.0);
    }

    SUBCASE("Power operator") {
        SquareMat c = a ^ 2;
        CHECK(c[0][0] == 7);
        CHECK(c[0][1] == 10);
        CHECK(c[1][0] == 15);
        CHECK(c[1][1] == 22);
    }

    // Test compound assignment operators

    SUBCASE("operator+=") {
        SquareMat x = a;
        x += b;
        CHECK(x[0][0] == 6);
        CHECK(x[0][1] == 8);
        CHECK(x[1][0] == 10);
        CHECK(x[1][1] == 12);
    }

    SUBCASE("operator-=") {
        SquareMat x = a;
        x -= b;
        CHECK(x[0][0] == -4);
        CHECK(x[0][1] == -4);
        CHECK(x[1][0] == -4);
        CHECK(x[1][1] == -4);
    }

    SUBCASE("operator*=(matrix)") {
        SquareMat x = a;
        x *= b;
        CHECK(x[0][0] == 19);
        CHECK(x[0][1] == 22);
        CHECK(x[1][0] == 43);
        CHECK(x[1][1] == 50);
    }

    SUBCASE("operator*=(scalar)") {
        SquareMat x = a;
        x *= 2.0;
        CHECK(x[0][0] == 2);
        CHECK(x[0][1] == 4);
        CHECK(x[1][0] == 6);
        CHECK(x[1][1] == 8);
    }

    SUBCASE("operator%=(matrix)") {
        SquareMat x = a;
        x %= b;
        CHECK(x[0][0] == 5);
        CHECK(x[0][1] == 12);
        CHECK(x[1][0] == 21);
        CHECK(x[1][1] == 32);
    }

    SUBCASE("operator%=(scalar)") {
        SquareMat x = a;
        x %= 3;
        CHECK(x[0][0] == 1);
        CHECK(x[0][1] == 2);
        CHECK(x[1][0] == 0);
        CHECK(x[1][1] == 1);
    }

    SUBCASE("operator/=(scalar)") {
        SquareMat x = b;
        x /= 2.0;
        CHECK(x[0][0] == 2.5);
        CHECK(x[0][1] == 3.0);
        CHECK(x[1][0] == 3.5);
        CHECK(x[1][1] == 4.0);
    }

    // Compound assignment operators (already tested)
    SUBCASE("Pre-increment") {
        SquareMat x = a;
        ++x;
        CHECK(x[0][0] == 2);
        CHECK(x[0][1] == 3);
        CHECK(x[1][0] == 4);
        CHECK(x[1][1] == 5);
    }

    SUBCASE("Post-increment") {
        SquareMat x = a;
        SquareMat before = x++;
        CHECK(before[0][0] == 1);
        CHECK(before[0][1] == 2);
        CHECK(before[1][0] == 3);
        CHECK(before[1][1] == 4);
        CHECK(x[0][0] == 2);
        CHECK(x[0][1] == 3);
        CHECK(x[1][0] == 4);
        CHECK(x[1][1] == 5);
    }

    SUBCASE("Pre-decrement") {
        SquareMat x = a;
        --x;
        CHECK(x[0][0] == 0);
        CHECK(x[0][1] == 1);
        CHECK(x[1][0] == 2);
        CHECK(x[1][1] == 3);
    }

    SUBCASE("Post-decrement") {
        SquareMat x = a;
        SquareMat before = x--;
        CHECK(before[0][0] == 1);
        CHECK(before[0][1] == 2);
        CHECK(before[1][0] == 3);
        CHECK(before[1][1] == 4);
        CHECK(x[0][0] == 0);
        CHECK(x[0][1] == 1);
        CHECK(x[1][0] == 2);
        CHECK(x[1][1] == 3);
    }

    // Test transpose operator (~)
    SUBCASE("Transpose operator") {
        SquareMat t = ~a;
        CHECK(t[0][0] == 1);
        CHECK(t[0][1] == 3);
        CHECK(t[1][0] == 2);
        CHECK(t[1][1] == 4);
    }

    // Test sum() helper
    SUBCASE("Sum helper") {
        CHECK(a.sum() == 10);
        CHECK(b.sum() == 26);
    }

    // Test comparison operators

    SUBCASE("Comparison == and !=") {
        SquareMat c(2);
        c[0][0] = 4; c[0][1] = 3;
        c[1][0] = 2; c[1][1] = 1;
        CHECK(a == c);
        CHECK_FALSE(a != c);
    }

    SUBCASE("Comparison < and <=") {
        SquareMat smaller(2);
        smaller[0][0] = 0; smaller[0][1] = 1;
        smaller[1][0] = 2; smaller[1][1] = 3;
        CHECK(smaller < a);
        CHECK(smaller <= a);
    }

    SUBCASE("Comparison > and >=") {
        SquareMat bigger(2);
        bigger[0][0] = 2; bigger[0][1] = 3;
        bigger[1][0] = 4; bigger[1][1] = 5;
        CHECK(bigger > a);
        CHECK(bigger >= a);
    }

    // Test getMinor()
    SUBCASE("getMinor helper") {
        SquareMat m(3);
        m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
        m[1][0] = 0; m[1][1] = 4; m[1][2] = 5;
        m[2][0] = 1; m[2][1] = 0; m[2][2] = 6;

        SquareMat minor = m.getMinor(0, 0);
        CHECK(minor[0][0] == 4);
        CHECK(minor[0][1] == 5);
        CHECK(minor[1][0] == 0);
        CHECK(minor[1][1] == 6);
    }

    // Test determinant operator (!)
    SUBCASE("Determinant operator") {
        CHECK((!a) == -2);

        SquareMat m2(3);
        m2[0][0] = 6; m2[0][1] = 1; m2[0][2] = 1;
        m2[1][0] = 4; m2[1][1] = -2; m2[1][2] = 5;
        m2[2][0] = 2; m2[2][1] = 8; m2[2][2] = 7;
        CHECK((!m2) == -306);
    }

    // Test element accessors
    SUBCASE("Element access") {
        SquareMat x(2);
        x[0][0] = 9;
        x[0][1] = 8;
        x[1][0] = 7;
        x[1][1] = 6;
        CHECK(x[0][0] == 9);
        CHECK(x[0][1] == 8);
        CHECK(x[1][0] == 7);
        CHECK(x[1][1] == 6);
    }

    // Friend functions

    SUBCASE("Scalar multiplication operator (scalar * matrix)") {
        SquareMat c = 2.0 * a;
        CHECK(c[0][0] == 2);
        CHECK(c[0][1] == 4);
        CHECK(c[1][0] == 6);
        CHECK(c[1][1] == 8);
    }

    // Test stream output (operator<<)
    SUBCASE("Stream output operator<<") {
        std::ostringstream oss;
        oss << a;
        std::string expected = "[ 1, 2 ]\n[ 3, 4 ]\n";
        CHECK(oss.str() == expected);
    }

}
