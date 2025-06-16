# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread -g --coverage
LDFLAGS = --coverage

# Folders
ALG_DIR = algorithms
CORE_DIR = core
SRV_DIR = server

# Headers (not actually needed for makefile rules but kept for documentation)
HDR = $(CORE_DIR)/Graph.hpp \
      $(CORE_DIR)/GraphAlgorithms.hpp \
      $(ALG_DIR)/AlgorithmStrategy.hpp \
      $(ALG_DIR)/AlgorithmFactory.hpp \
      $(ALG_DIR)/MSTAlgorithm.hpp \
      $(ALG_DIR)/SCCAlgorithm.hpp \
      $(ALG_DIR)/CliqueAlgorithm.hpp \
      $(ALG_DIR)/MaxFlowAlgorithm.hpp \
      $(SRV_DIR)/BlockingQueue.hpp \
      $(SRV_DIR)/PipelineData.hpp

# Source files
GRAPH_SRC = $(CORE_DIR)/Graph.cpp $(CORE_DIR)/GraphAlgorithms.cpp \
            $(ALG_DIR)/AlgorithmFactory.cpp

SERVER_SRC = $(SRV_DIR)/Server.cpp

# Build object files from sources
GRAPH_OBJ = $(GRAPH_SRC:.cpp=.o)
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)

# Final executable
SERVER_OUT = server_exec

# Default rule
all: $(SERVER_OUT)

# Link the final executable
$(SERVER_OUT): $(SERVER_OBJ) $(GRAPH_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile each cpp file into object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(SERVER_OUT) $(GRAPH_OBJ) $(SERVER_OBJ) *.gcda *.gcno *.info
	rm -rf coverage

# Coverage rule
coverage: $(SERVER_OUT)
	lcov --capture --directory $(CORE_DIR) --directory $(ALG_DIR) --directory $(SRV_DIR) --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	@echo "Coverage report generated: ./coverage/index.html"

