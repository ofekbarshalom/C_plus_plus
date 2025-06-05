# Compiler and compilation flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror

# Header and source files
HEADERS = ConvexHullVector.hpp ConvexHullDeque.hpp Point.hpp Reactor.hpp
SERVER_SRC = Server_Reactor.cpp ConvexHullVector.cpp ConvexHullDeque.cpp Reactor.cpp
SERVER_TARGET = server

# Default target: build the server
all: $(SERVER_TARGET)

# Build the server executable
$(SERVER_TARGET): $(SERVER_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -o $(SERVER_TARGET)

# Run the server
run: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Remove build output
clean:
	rm -f $(SERVER_TARGET)
