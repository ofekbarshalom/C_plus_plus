# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror

# Header and source files
HEADERS = ConvexHullVector.hpp Point.hpp
MAIN_SRC = main.cpp ConvexHullVector.cpp
SERVER_SRC = Server.cpp ConvexHullVector.cpp

MAIN_TARGET = Main
SERVER_TARGET = server

# Default target: build both Main and server
all: $(MAIN_TARGET) $(SERVER_TARGET)

# Build CLI Main
$(MAIN_TARGET): $(MAIN_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) -o $(MAIN_TARGET)

# Build TCP Server
$(SERVER_TARGET): $(SERVER_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_TARGET)

# Run the server
run: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Clean all binaries
clean:
	rm -f $(MAIN_TARGET) $(SERVER_TARGET)
