CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

# Targets
all: signal_sender signal_receiver

signal_sender: signal_sender.cpp
	$(CXX) $(CXXFLAGS) -o signal_sender signal_sender.cpp

signal_receiver: signal_receiver.cpp
	$(CXX) $(CXXFLAGS) -o signal_receiver signal_receiver.cpp

clean:
	rm -f signal_sender signal_receiver
