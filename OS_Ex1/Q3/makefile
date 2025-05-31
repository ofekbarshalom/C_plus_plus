# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -fPIC

# Source files and targets
LIBSRC = mandelbrot.cpp
LIBHDR = mandelbrot.h
LIBOBJ = libmandelbrot.so

MAIN = q3.cpp
TARGET = q3

# Default target
all: $(TARGET)

# Build the shared library
$(LIBOBJ): $(LIBSRC) $(LIBHDR)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(LIBSRC)

# Build the main program and link it with the shared library
$(TARGET): $(MAIN) $(LIBOBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(MAIN) -L. -lmandelbrot -Wl,-rpath=.
	
# Clean rule
clean:
	rm -f *.o *.so $(TARGET)

.PHONY: clean all

