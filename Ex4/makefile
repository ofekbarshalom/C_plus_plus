# ofekbarshalom@gmail.com

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

CODE_DIR = code
TEST_DIR = tests

SRC_FILES = $(CODE_DIR)/main.cpp
TEST_FILES = $(TEST_DIR)/test.cpp
HEADERS = $(wildcard $(CODE_DIR)/*.hpp)

MAIN_EXE = Main
TEST_EXE = Test

# Build the main demo executable
Main: $(SRC_FILES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXE) $(SRC_FILES)

# Build and run tests
test: $(TEST_FILES) $(HEADERS) $(TEST_DIR)/doctest.h
	$(CXX) $(CXXFLAGS) -o $(TEST_EXE) $(TEST_FILES)
	./$(TEST_EXE)

# Run valgrind on main program
valgrind: Main
	$(VALGRIND) ./$(MAIN_EXE)

# Clean all generated files
clean:
	rm -f $(MAIN_EXE) $(TEST_EXE) *.o
