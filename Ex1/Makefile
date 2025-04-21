# ofekbarshalom@gmail.com

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# Sources
SRCS = main.cpp Graph.cpp Algorithms.cpp Queue.cpp UnionFind.cpp
TEST_SRCS = test.cpp Graph.cpp Algorithms.cpp Queue.cpp UnionFind.cpp

# Executables
MAIN_EXE = Main
TEST_EXE = Test

# Build the main program
Main: $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXE) $(SRCS)

# Build and run tests
test: $(TEST_SRCS) doctest.h
	$(CXX) $(CXXFLAGS) -o $(TEST_EXE) $(TEST_SRCS)
	./$(TEST_EXE)

# Run valgrind on the main executable
valgrind: Main
	$(VALGRIND) ./$(MAIN_EXE)

# Clean build files
clean:
	rm -f $(MAIN_EXE) $(TEST_EXE) *.o
