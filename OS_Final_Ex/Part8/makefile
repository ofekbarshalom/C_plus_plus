# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

# Common files
HDR = Graph.hpp GraphAlgorithms.hpp AlgorithmStrategy.hpp \
      AlgorithmFactory.hpp EulerianAlgorithm.hpp MSTAlgorithm.hpp \
      SCCAlgorithm.hpp CliqueAlgorithm.hpp MaxFlowAlgorithm.hpp

GRAPH_SRC = Graph.cpp GraphAlgorithms.cpp AlgorithmFactory.cpp

# Main executable
MAIN_SRC = main.cpp
MAIN_OUT = main

# Server executable
SERVER_SRC = Server.cpp
SERVER_OUT = server

# Build main
$(MAIN_OUT): $(MAIN_SRC) $(GRAPH_SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(MAIN_OUT) $(MAIN_SRC) $(GRAPH_SRC)

# Build server
$(SERVER_OUT): $(SERVER_SRC) $(GRAPH_SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(SERVER_OUT) $(SERVER_SRC) $(GRAPH_SRC)

# Clean rule
clean:
	rm -f $(MAIN_OUT) $(SERVER_OUT)
