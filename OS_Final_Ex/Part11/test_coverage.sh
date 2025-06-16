#!/bin/bash

set -e

# Required for coverage flushing properly:
export GCOV_PREFIX=.
export GCOV_PREFIX_STRIP=0

echo "Cleaning previous builds..."
make clean

echo "Rebuilding project with coverage flags..."
make

echo "Running full test session..."

# Start the server directly in foreground (no &)
./server_exec &

SERVER_PID=$!

sleep 1

# Run clients while server is running

# First client (manual graph)
(
echo "1"
sleep 0.1
echo "6 8 0"
echo "0 1"
echo "1 2"
echo "2 3"
echo "3 0"
echo "1 3"
echo "3 4"
echo "4 5"
echo "5 3"
sleep 1
) | nc localhost 8080

# Second client (random graph)
(
echo "2"
sleep 0.1
echo "6 8 123 0"
sleep 1
) | nc localhost 8080

# Third client (exit)
(
echo "0"
) | nc localhost 8080

# Let the server fully finish
sleep 2

# Now graceful shutdown
kill -SIGINT $SERVER_PID
wait $SERVER_PID

echo "Collecting coverage..."
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage

echo "✅ Coverage report generated at ./coverage/index.html"
