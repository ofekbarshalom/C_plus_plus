# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror

# Header and source files
HEADERS = ConvexHullVector.hpp ConvexHullDeque.hpp Point.hpp
SRC = main.cpp ConvexHullVector.cpp ConvexHullDeque.cpp
TARGET = Main

# Default target
all: $(TARGET)

# Build rule
$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Run target
run: $(TARGET)
	./$(TARGET)

# Clean target
clean:
	rm -f $(TARGET)
