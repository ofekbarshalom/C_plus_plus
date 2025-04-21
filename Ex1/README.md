# Graph Algorithms Project

**Author**: Ofek Bar Shalom  
**Email**: ofekbarshalom@gmail.com  
**Course**: תכנות מערכות 2  
**Assignment**: מטלה 1 – מימוש גרף על ידי רשימת שכנויות

---

##  Project Overview

This project implements a **graph data structure** using an **adjacency list** in C++ without using STL containers.  
It also includes implementations of **classical graph algorithms**, memory management, and test validation using `doctest`.

---

##  File Structure

| File             | Description |
|------------------|-------------|
| `Graph.hpp/cpp`      | Implements the `Graph` class using dynamic arrays (no STL). Contains edge manipulation, adjacency list, and utility functions. |
| `Algorithms.hpp/cpp` | Implements classic graph algorithms: BFS, DFS, Dijkstra, Prim, Kruskal. |
| `Queue.hpp/cpp`      | A simple queue struct using a circular buffer. Used for BFS. |
| `UnionFind.hpp/cpp`  | Implements the Union-Find (Disjoint Set Union) data structure. Used for Kruskal. |
| `main.cpp`           | Demonstrates usage of the `Graph` and algorithms on example data. |
| `test.cpp`           | Contains unit tests for all major components using the `doctest` framework. |
| `doctest.h`          | Header-only testing framework for C++. |
| `Makefile`           | Allows compilation, testing, cleaning, and memory leak checking. |

---

##  Implemented Algorithms

- **Breadth-First Search (BFS)** – returns a BFS tree starting from a given vertex.
- **Depth-First Search (DFS)** – returns a DFS tree/forest starting from a given vertex.
- **Dijkstra's Algorithm** – builds a shortest-path tree from a single source.
- **Prim's Algorithm** – constructs the minimum spanning tree (MST).
- **Kruskal's Algorithm** – constructs the MST using Union-Find.

---

##  Building and Running

Use the provided **Makefile** for all operations:

```bash
make Main         # Build the demo (main.cpp)
./Main            # Run and see algorithm output

make test         # Run unit tests (test.cpp with doctest)
make valgrind     # Run Valgrind to check for memory leaks
make clean        # Clean up build artifacts
