# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread -g --coverage
LDFLAGS = --coverage

# Folders
ALG_DIR = algorithms
CORE_DIR = core
SRV_DIR = server

# Headers
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

# Sources
GRAPH_SRC = $(CORE_DIR)/Graph.cpp $(CORE_DIR)/GraphAlgorithms.cpp \
            $(ALG_DIR)/AlgorithmFactory.cpp

SERVER_SRC = $(SRV_DIR)/Server.cpp
SERVER_OUT = server_exec

# Build
$(SERVER_OUT): $(SERVER_SRC) $(GRAPH_SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(SERVER_OUT) $(SERVER_SRC) $(GRAPH_SRC) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(SERVER_OUT) *.gcda *.gcno *.info
	rm -rf coverage

# Coverage rule
coverage: $(SERVER_OUT)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	@echo "Coverage report generated: ./coverage/index.html"
