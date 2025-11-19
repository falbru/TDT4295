# TDT4295

The MCU code for our project in TDT4295 Computer Design Project. Skidaddle is an embedded solution where you draw on a drawing board and a machine learning model tries to predict what you're drawing. The project encompasses a PCB, MCU and FPGA.

## Dependencies

- GCC compiler
- Make
- CMake (version 3.12 or higher)
- SDL3 (for desktop platform only)

## Getting started

1. Clone the repository

```sh
git clone https://github.com/falbru/TDT4295
cd TDT4295
```

2. Initialize submodules (for Unity testing framework)

```sh
git submodule update --init
```

3. Create build directory

```sh
mkdir build
```

4. Configure the project

```sh
cmake -S . -B build
```

Build options:
- `BUILD_TESTS` - Build tests for libgui and libgame
- `BUILD_DESKTOP` - Build desktop simulator (requires SDL3)

5. Build

```sh
cmake --build build
```

## Testing

We use Unity as our testing framework. To run the tests, ensure that the project is built with `BUILD_TESTS` set to `ON`. Then use `ctest` to run all the tests:

```sh
ctest --test-dir build --output-on-failure
```
