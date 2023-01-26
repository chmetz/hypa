# HyPA

## Build

### Prerequisites

- [cmake](https://cmake.org/)
- compatible C++ 20 Compiler (e.g. gcc)
- [boost](https://www.boost.org/)

### Compile

The easiest way to build HyPA is by using the following commands:
```bash
mkdir build
cmake ..
cd build
make
```

### Configurations

- BUILD_TESTS
  - build the CTest Unit Tests
- DEBUG
  - use the compiler flags `-g`, `-Wall` and `-Wextra`

#### Example on how to use the configuration variables

```bash
mkdir build
cmake -DBUILD_TESTS=0 -DDEBUG=0 ..
cd build
make
```

## Use

With the building process from above you will create an executable called `hypa` in the build directory.
Now you can use this on the CLI to start the program.
There are several CLI options which are optional and listed below, but you always have to specify a path to a directory which contains PTX Files or directly to one PTX File.

#### Example for analyzing all ptx files contained in this repository
```bash
./hypa -csv ../resources
```

### CLI Options

- -s: skip the dynamic analysis and only execute the statical part
- -oT: only execute one thread for every CTA -> faster but more inaccurate
- -csv: create a csv file instead of writing the results to the stdout
- -g: if this option is set, HyPA will create dot files containing the instruction graph
- -v: enabling verbose output for debugging purposes