# Compiler and flags
CXX = g++
AR = ar
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pthread

# Header and source files
HEADERS = ConvexHullVector.hpp ConvexHullDeque.hpp Point.hpp Reactor.hpp Proactor.hpp
MAIN_SRC = main.cpp ConvexHullVector.cpp ConvexHullDeque.cpp
SERVER_SRC = Server.cpp ConvexHullVector.cpp ConvexHullDeque.cpp

REACTOR_PROACTOR_SRC = Reactor.cpp Proactor.cpp
REACTOR_PROACTOR_LIB = libreaction.a

MAIN_TARGET = Main
SERVER_TARGET = server

# Default target: build all
all: $(REACTOR_PROACTOR_LIB) $(MAIN_TARGET) $(SERVER_TARGET)

# Build library from Reactor/Proactor source
$(REACTOR_PROACTOR_LIB): $(REACTOR_PROACTOR_SRC)
	$(CXX) $(CXXFLAGS) -c $(REACTOR_PROACTOR_SRC)
	$(AR) rcs $(REACTOR_PROACTOR_LIB) Reactor.o Proactor.o

# Build CLI client
$(MAIN_TARGET): $(MAIN_SRC) $(HEADERS) $(REACTOR_PROACTOR_LIB)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) -L. -lreaction -o $(MAIN_TARGET)

# Build TCP server
$(SERVER_TARGET): $(SERVER_SRC) $(HEADERS) $(REACTOR_PROACTOR_LIB)
	$(CXX) $(CXXFLAGS) $(SERVER_SRC) -L. -lreaction -o $(SERVER_TARGET)

# Run the server
run: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Clean all
clean:
	rm -f *.o *.a $(MAIN_TARGET) $(SERVER_TARGET)
