# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Output
TARGET = libreactor.a

# Source and object files
SRC = Reactor.cpp
OBJ = $(SRC:.cpp=.o)

# Default rule
all: $(TARGET)

# Create static library
$(TARGET): $(OBJ)
	ar rcs $@ $^

# Compile source files into object files
%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Optional install target (change path as needed)
install: $(TARGET)
	cp $(TARGET) /usr/local/lib/
	cp Reactor.hpp /usr/local/include/

.PHONY: all clean install
