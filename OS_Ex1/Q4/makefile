# Files
SRC = dijkstra.cpp
BIN = dijkstra

# Default target
all:
	g++ -fprofile-arcs -ftest-coverage $(SRC) -o $(BIN)

# Clean target
clean:
	rm -f $(BIN) *.gcno *.gcda *.gcov core
