# List of all parts
PARTS = Part1 Part2 Part3 Part4 Part5 Part6 Part7 Part8 Part9 Part10

# Default target: build all parts
all: $(PARTS)

# Rule to delegate to each subdirectory
$(PARTS):
	$(MAKE) -C $@

# Clean all parts
clean:
	for dir in $(PARTS); do \
		$(MAKE) -C $$dir clean; \
	done
