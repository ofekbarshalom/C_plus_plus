# SquareMat Matrix Project

**Author**: Ofek Bar Shalom  
**Email**: ofekbarshalom@gmail.com  
**Course**: תכנות מערכות 2  
**Assignment**: מטלה 2 – מימוש מטריצות ריבועיות ואופרטורים

---

##  Project Overview

This project implements a **Square Matrix (`SquareMat`)** class in C++ using dynamic memory allocation and **overloaded operators**.  
The implementation covers a wide range of matrix operations without using the STL. It also includes **unit tests** with `doctest`.

---

##  File Structure

| File             | Description |
|------------------|-------------|
| `SquareMat.hpp/cpp` | Implements the `SquareMat` class with dynamic memory, operator overloading and helper functions. |
| `main.cpp`           | Demonstrates the use of all the matrix features and overloaded operators. |
| `test.cpp`           | Contains automated unit tests using the `doctest` framework. |
| `doctest.h`          | Header-only C++ testing framework. |
| `Makefile`           | Compilation, testing, valgrind, and cleanup commands. |

---

##  Implemented Features

- Arithmetic operators: `+`, `-`, `*`, `/`, `%`, `^`
- Compound assignment: `+=`, `-=`, `*=`, `/=`, `%=`
- Unary operators: `-`, `~` (transpose), `!` (determinant)
- Scalar operations: Matrix × Scalar, Scalar × Matrix
- Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=` (by sum of elements)
- Increment / Decrement: `++`, `--` (pre/post)
- Helper methods: `sum()`, `getMinor(row, col)`
- Output: Stream-friendly `operator<<`
- Manual 2D element access via `matrix[i][j]`

---

##  Building and Running

Use the provided **Makefile** for all common operations:

```bash
make Main         # Compile and run the main demo (main.cpp)
./Main

make test         # Compile and run tests (test.cpp + doctest)
make valgrind     # Run Valgrind memory analysis on Main
make clean        # Clean all build files
