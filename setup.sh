#!/bin/sh

[ -z "$CXXTEST" ] && echo "The CXXTEST environment variable is not set, please set it to where cxxtest's include directory is (e.g. /usr/include/cxxtest)" && exit 1

[ ! -f "$CXXTEST/cxxtest/TestSuite.h" ] && echo "The CXXTEST environment variable does not seem to point to a folder with the relevant headers inside of it." && exit 1

mkdir -p bin
python3 metamake/main.py > makefile

