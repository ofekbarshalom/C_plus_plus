# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread -g

# Folders
ALG_DIR = algorithms
CORE_DIR = core
SRV_DIR = server

# Header files
HDR = $(CORE_DIR)/Graph.hpp \
      $(CORE_DIR)/GraphAlgorithms.hpp \
      $(ALG_DIR)/AlgorithmStrategy.hpp \
      $(ALG_DIR)/AlgorithmFactory.hpp \
      $(ALG_DIR)/EulerianAlgorithm.hpp \
      $(ALG_DIR)/MSTAlgorithm.hpp \
      $(ALG_DIR)/SCCAlgorithm.hpp \
      $(ALG_DIR)/CliqueAlgorithm.hpp \
      $(ALG_DIR)/MaxFlowAlgorithm.hpp \
      $(SRV_DIR)/BlockingQueue.hpp \
      $(SRV_DIR)/PipelineData.hpp

# Source files
GRAPH_SRC = $(CORE_DIR)/Graph.cpp $(CORE_DIR)/GraphAlgorithms.cpp \
            $(ALG_DIR)/AlgorithmFactory.cpp

# Main executable
MAIN_SRC = main.cpp
MAIN_OUT = main

# Server executable
SERVER_SRC = $(SRV_DIR)/Server.cpp
SERVER_OUT = server_exec

# Build main
$(MAIN_OUT): $(MAIN_SRC) $(GRAPH_SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(MAIN_OUT) $(MAIN_SRC) $(GRAPH_SRC)

# Build server_exec
$(SERVER_OUT): $(SERVER_SRC) $(GRAPH_SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(SERVER_OUT) $(SERVER_SRC) $(GRAPH_SRC)

# Alias target
.PHONY: server valgrind helgrind callgrind clean
server: $(SERVER_OUT)

# Run Valgrind memcheck
valgrind: $(SERVER_OUT)
	valgrind --leak-check=full --show-leak-kinds=all ./$(SERVER_OUT)

# Run Helgrind (thread race detection)
helgrind: $(SERVER_OUT)
	valgrind --tool=helgrind ./$(SERVER_OUT)

# Run Callgrind (performance profiling)
callgrind: $(SERVER_OUT)
	valgrind --tool=callgrind ./$(SERVER_OUT)

# Clean rule
clean:
	rm -f $(MAIN_OUT) $(SERVER_OUT) callgrind.out.*
