# Molecule bar – Operating Systems Assignment (EX2)

This project implements a multi-stage system for managing atoms, generating molecules, and simulating a drinks bar server.

## Project Structure

The project is divided into 6 parts:

- **Part1** – Basic TCP server for adding atoms
- **Part2** – Adds UDP support for molecule delivery
- **Part3** – Adds console input for drink generation
- **Part4** – Adds startup options (atoms count, ports, timeout)
- **Part5** – Adds support for UDS (Unix Domain Sockets)
- **Part6** – Adds persistent inventory support (shared memory + file + Bonus)

Each part resides in its own subdirectory: `Part1/`, `Part2/`, ..., `Part6/`.  
Each subdirectory includes its own `makefile` for compilation.

## Code Coverage

Each part includes a code coverage report.  
These reports are stored in the `coverage/` directory and describe which parts of the code were tested and how.

## Building the Project

To build **all parts at once**, from the root directory run:

```bash
make all
```

To clean all executables in all parts, run:

```bash
make clean
```
