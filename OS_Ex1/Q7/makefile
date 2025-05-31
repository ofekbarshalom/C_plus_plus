# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++20

# Targets (executables)
TARGETS = findPhone add2PB

# Default target: build everything
all: $(TARGETS)

# Build findPhone
findPhone: findPhone.cpp
	$(CXX) $(CXXFLAGS) findPhone.cpp -o findPhone

# Build add2PB
add2PB: add2PB.cpp
	$(CXX) $(CXXFLAGS) add2PB.cpp -o add2PB

# Clean: remove compiled binaries
clean:
	rm -f $(TARGETS)
