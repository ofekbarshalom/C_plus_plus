# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

# Files
SRC = main.cpp Graph.cpp GraphAlgorithms.cpp
HDR = Graph.hpp GraphAlgorithms.hpp
OUT = main

# Build rule
$(OUT): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Clean rule
clean:
	rm -f $(OUT)

