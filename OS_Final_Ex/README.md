# Graph Algorithm Server

This project implements a multithreaded C++ TCP server that allows users to input or generate graphs and compute multiple graph algorithms in parallel using the pipeline pattern.

> The file structure and explanation apply specifically to **Part 9** of the project.  
> Earlier parts (1–8) follow separate implementation instructions and may differ in structure.

## Features

- TCP server that accepts client connections.
- Graph input: 
  - Manual entry
  - Random generation
- Parallel execution of 4 graph algorithms:
  - Minimum Spanning Tree (MST)
  - Strongly Connected Components (SCC)
  - Maximum Clique
  - Maximum Flow (Edmonds-Karp)

## Architecture

- **Client-Server Communication**: The server listens on port `8080` and interacts with clients via sockets.
- **Concurrency**: Each algorithm runs on a separate thread and results are sent back once all computations are complete.
- **Graph Representation**: Uses an adjacency list implemented with STL (vector), encapsulated in a custom Graph class.
- **Algorithm Factory**: Implements a factory pattern to instantiate different algorithms dynamically.
- **Strategy Pattern**: Each algorithm follows the Strategy interface, allowing interchangeable execution logic.

## Input Formats

### Manual Graph Entry
The client provides:
```
vertices edges directed_flag(0/1)
u1 v1
u2 v2
...
```

### Random Graph Generation
The client provides:
```
vertices edges seed directed_flag(0/1)
```

## Build Instructions

```bash
make server
```

## Run Server

```bash
./server_exec
```

## Run Client (example via netcat)

```bash
nc localhost 8080
```

## Example Session

```
Choose input mode:
0 - Exit server
1 - Generate random graph
2 - Enter graph manually
1
Enter: vertices edges seed directed(0=dir,1=undir):
6 7 42 1
```

## Output Example

```
MST Weight: 13
SCC Components (3):
{ 0 1 }
{ 2 }
{ 3 4 5 }
Max Clique (3): 0 2 4 
Max Flow: 9
```

## File Structure

```
├── algorithms/
│   ├── AlgorithmFactory.cpp / .hpp      # Factory pattern to instantiate algorithms
│   ├── AlgorithmStrategy.hpp            # Base strategy interface
│   ├── CliqueAlgorithm.hpp              # Max clique strategy
│   ├── EulerianAlgorithm.hpp            # Euler circuit strategy
│   ├── MaxFlowAlgorithm.hpp             # Max flow strategy
│   ├── MSTAlgorithm.hpp                 # MST weight strategy
│   └── SCCAlgorithm.hpp                 # SCC strategy
│
├── core/
│   ├── Graph.cpp / .hpp                 # Graph structure using adjacency list
│   └── GraphAlgorithms.cpp / .hpp       # Central algorithm implementations
│
├── server/
│   ├── BlockingQueue.hpp                # Thread-safe queue for pipeline
│   ├── PipelineData.hpp                 # Struct passed between pipeline stages
│   └── Server.cpp                       # TCP server handling client requests
│
├── main.cpp                             # CLI tool to generate and test graphs
├── makefile                             # Build script
```


## Dependencies

- C++20
- POSIX socket API (Linux)
- `make`

## Authors

- Developed as part of the final OS course project.

## License

MIT License
