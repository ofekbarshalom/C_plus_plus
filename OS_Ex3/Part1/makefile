CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror

SRC = main.cpp ConvexHull.cpp
TARGET = Main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
