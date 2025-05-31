# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -pedantic -std=c++20

# Q1 programs to compile
Q1_SRCS := Q1/codes/Division_by_zero.cpp Q1/codes/Invalid_memory_address.cpp Q1/codes/Stack_overflow.cpp
Q1_BINS := $(Q1_SRCS:.cpp=)  # Remove .cpp to get the binary names

# Directories for Q2–Q7
SUBDIRS := Q2 Q3 Q4 Q5 Q6 Q7

# Default target: build Q1 binaries and run make in Q2–Q7
all: $(Q1_BINS)
	@for dir in $(SUBDIRS); do \
		if [ -f $$dir/makefile ] || [ -f $$dir/Makefile ]; then \
			echo "Building $$dir..."; \
			$(MAKE) -C $$dir; \
		else \
			echo "Skipping $$dir (no Makefile)"; \
		fi; \
	done

# Compile each Q1 source file into its corresponding binary
Q1/codes/%: Q1/codes/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean everything
clean:
	@for bin in $(Q1_BINS); do \
		rm -f $$bin; \
	done
	@for dir in $(SUBDIRS); do \
		if [ -f $$dir/makefile ] || [ -f $$dir/Makefile ]; then \
			echo "Cleaning $$dir..."; \
			$(MAKE) -C $$dir clean; \
		fi; \
	done
