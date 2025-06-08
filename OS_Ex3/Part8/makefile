# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pthread

# Files
PROACTOR_SRC = Proactor.cpp
PROACTOR_OBJ = $(PROACTOR_SRC:.cpp=.o)
PROACTOR_LIB = libproactor.a
HEADERS = Proactor.hpp

# Default target: build the library
all: $(PROACTOR_LIB)

# Compile object file
$(PROACTOR_OBJ): $(PROACTOR_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $(PROACTOR_SRC) -o $(PROACTOR_OBJ)

# Archive object into static library
$(PROACTOR_LIB): $(PROACTOR_OBJ)
	ar rcs $@ $^

# Clean build files
clean:
	rm -f $(PROACTOR_OBJ) $(PROACTOR_LIB)
