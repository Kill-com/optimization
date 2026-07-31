#!/bin/bash

EXTRA_ARGS=""
[ "$1" = "-s" ] && EXTRA_ARGS="-s"

if [ -d "build" ]; then
    cd build
    cmake --build .
    cd ..
    sudo ./optimization -f target_function/compiled/f_comp/ -m methods/compiled/methods_comp/ $EXTRA_ARGS
else
    mkdir build && cd build
    cmake .. && make
    cd ..
    cp build/compile_commands.json .
fi