CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I./src -I./GUI
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = $(wildcard src/*.cpp) GUI/GUI.cpp main.cpp
TARGET = main

Main: $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(LDFLAGS)

run: Main
	./$(TARGET)

clean:
	rm -f $(TARGET)
