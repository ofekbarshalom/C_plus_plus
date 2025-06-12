# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g

SRC = main.cpp Graph.cpp GraphAlgorithms.cpp
HDR = Graph.hpp GraphAlgorithms.hpp
OUT = main

# Normal build
$(OUT): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

# Run with example args
run: $(OUT)
	./$(OUT) -v 5 -e 6 -s 123

# Clean all build and report artifacts
clean:
	rm -f $(OUT) main_coverage main_gprof gmon.out callgrind.out.* *.gcda *.gcno *.gcov *.txt *.o

# Analyze Phase 1: Build with coverage + gprof flags
analyze_build: clean
	$(CXX) -std=c++17 -fprofile-arcs -ftest-coverage -pg -g -c main.cpp
	$(CXX) -std=c++17 -fprofile-arcs -ftest-coverage -pg -g -c Graph.cpp
	$(CXX) -std=c++17 -fprofile-arcs -ftest-coverage -pg -g -c GraphAlgorithms.cpp
	$(CXX) -fprofile-arcs -ftest-coverage -pg -g -o main main.o Graph.o GraphAlgorithms.o
	
# Analyze Phase 2: Generate reports after you run main manually
analyze_report:
	gcov -b main.cpp Graph.cpp GraphAlgorithms.cpp > coverage_report.txt
	gprof main gmon.out > gprof_report.txt
	-valgrind --leak-check=full ./main -v 1000 -e 3000 -s 123 2> valgrind_memcheck.txt
	-valgrind --tool=callgrind ./main -v 1000 -e 3000 -s 123
	callgrind_annotate callgrind.out.* > callgraph.txt
