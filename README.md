# Project Compilers 2021

Infix calculator and compiler to c.

## Requirements

The project requires:

- `flex` and `bison` for the parser
- `readline` to use the interpreter
- `CMake` as build tool
- **optionally** `ninja` and `gnu-make` also for building

## Build

Use CMake to build the project. There is also a Makefile to automate the
commands but it requires `Ninja` to build. The output files will be in the
`build` directory.

```bash
# With ninja just use make
make
# Without do the usual CMake stuff
cmake -S . --B build
cmake --build build
```
