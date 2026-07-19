#!/bin/bash

EXTRA_ARGS=""
[ "$1" = "-s" ] && EXTRA_ARGS="-s"

if [ -d "build" ]; then
    ./optimization -f target_function/compiled/f_comp/ -m methods/compiled/methods_comp/ $EXTRA_ARGS
else
    mkdir build && cd build
    cmake .. && make
    cd ..
fi