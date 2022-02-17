# hottellbt capstone project

this is the repository for benjamin hottell's (hottellbt) computer science capstone

## languages

C++ is the main language for the project. Python is used to generate C++ code when appropriate, and to help with compilation. Therefore, Python is a dependency for building and testing. Once compiled, the executable does not need Python.

## build & run & test

- run `setup.sh` to generate a makefile
- use `make run` or `make test` to run or test respectively
  - if you intend on running tests, you need to install [CxxTest](https://cxxtest.com/)
- if you ever add `#include` statements, new source files, or new header files, run `setup.sh` again to get an updated makefile

makefile generation is handled in the `metamake` directory.
