# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

# Files
SRC = main.cpp Graph.cpp
HDR = Graph.hpp
OUT = main

# Build rule
$(OUT): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Run rule
run: $(OUT)
	./$(OUT)

# Clean rule
clean:
	rm -f $(OUT)

