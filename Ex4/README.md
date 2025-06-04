# Container and Iterators Project (EX4)

A templated container class that supports multiple custom iteration orders over its elements.

## Features

- Generic container implementation using `template<typename T>`
- Supports adding and removing elements
- Custom iterator implementations:
  - **AscendingOrderIterator** – Traverse elements in ascending order
  - **DescendingOrderIterator** – Traverse elements in descending order
  - **SideCrossOrderIterator** – Alternate between smallest/largest remaining
  - **ReverseOrderIterator** – Traverse in reverse of insertion order
  - **OrderIterator** – Traverse in insertion order
  - **MiddleOutOrderIterator** – Traverse from the middle, alternating outward

## File Structure

```
.
├── src/
│   ├── Container.hpp
│   ├── IterAsc.hpp
│   ├── IterDesc.hpp
│   ├── IterSideCross.hpp
│   ├── IterReverse.hpp
│   ├── IterOrder.hpp
│   └── IterMiddleOut.hpp
│   └── main.cpp
├── tests/
|   └── test.cpp
|   └── doctest.h
├── Makefile
└── README.md
```

## How to Build & Run

### Build the main program:
```bash
make Main
```

### Run with Valgrind (for memory leak checks):
```bash
make valgrind
```

### Run unit tests:
```bash
make test
```

## Example Output
```
[ Integer container ]
Elements: [ 7 15 6 1 2 ]
Ascending: 1 2 6 7 15
Descending: 15 7 6 2 1
Side-Cross: 1 15 2 7 6
Reverse: 2 1 6 15 7
Order: 7 15 6 1 2
Middle-Out: 6 15 1 7 2
```

## Requirements

- C++20 compatible compiler (e.g. `g++`)
- [doctest](https://github.com/doctest/doctest) header (already included)

## License

This project is provided for educational purposes.
