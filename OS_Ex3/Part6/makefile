# Compiler and compilation flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror

# Library output
REACTOR_LIB = libreactor.a

# Source and header files
HEADERS = ConvexHullVector.hpp Point.hpp Reactor.hpp
REACTOR_SRC = Reactor.cpp
REACTOR_OBJ = $(REACTOR_SRC:.cpp=.o)

SERVER_SRC = Server_Reactor.cpp ConvexHullVector.cpp
SERVER_TARGET = server

# Default target: build everything
all: $(REACTOR_LIB) $(SERVER_TARGET)

# Create static library for Reactor
$(REACTOR_LIB): $(REACTOR_OBJ)
	ar rcs $@ $^

# Build the server executable (linked with reactor lib)
$(SERVER_TARGET): $(SERVER_SRC) $(REACTOR_LIB) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -L. -lreactor -o $(SERVER_TARGET)

# Run the server
run: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Clean build artifacts
clean:
	rm -f $(SERVER_TARGET) $(REACTOR_OBJ) $(REACTOR_LIB)
