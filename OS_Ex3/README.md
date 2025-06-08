# 🧠 OS - Exercise 3: Convex Hull Server with Reactor Pattern

This project implements a **multi-client TCP server** that processes geometric commands (like computing the Convex Hull area) using a **reactor-based event loop**.

## 🚀 Features

- Supports multiple concurrent TCP clients (non-blocking I/O via `select()`)
- Handles geometric commands: `Newgraph`, `Newpoint`, `Removepoint`, `CH`
- Computes the convex hull using Andrew’s Monotone Chain algorithm
- Returns convex hull area per client request
- Modular C++ design with clear separation of logic

## 📁 Project Structure

```
.
├── ConvexHullVector.cpp/.hpp   # Convex Hull (vector implementation)
├── ConvexHullDeque.cpp/.hpp    # (optional) deque-based alternative
├── Point.hpp                   # 2D point representation
├── Reactor.cpp/.hpp            # Reactor pattern implementation
├── Server_Reactor.cpp          # Main server logic (entry point)
├── Makefile                    # Build script
```

## 💬 Supported Commands (via TCP socket)

| Command             | Description                                 |
|---------------------|---------------------------------------------|
| `Newgraph N`        | Start a new graph and expect `N` points next |
| `Newpoint X,Y`      | Add a new point to the current graph        |
| `Removepoint X,Y`   | Remove the specified point if it exists     |
| `CH`                | Compute the convex hull and return its area |

Each command must be sent **as a full line ending with `\n`**.

## ⚙️ How to Build

```bash
make
```

## 🧪 How to Run

```bash
make run
```

The server will listen on port **9034**.

## 🔄 Example Interaction

You can test the server using `netcat`:

```bash
nc localhost 9034
```

Then enter:

```
Newgraph 3
0,0
1,0
0,1
CH
```

## 🧹 Clean Build

```bash
make clean
```

## 📦 Dependencies

- C++20
- Linux or WSL (POSIX sockets and `select()`)

## 🔧 Known Limitations

- No thread safety (single-threaded reactor)
- Assumes each client sends one complete command per line
- Minimal error handling
