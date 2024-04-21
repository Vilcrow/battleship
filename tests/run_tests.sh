#!/bin/bash
# Compiles and runs the tests

# TODO: Need create cycle for compiling and testing in all directories
echo "[Compilation...]"
if make
then
    echo "[Tests compiled]"
else
    echo "[Compilation failed]"
    exit 1
fi

echo "[Testing...]"
./core_test -c -v
