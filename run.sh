#!/bin/bash

COMPILE_ONLY=0
RUN_ONLY=0
CLEAN=0
EXTRA_ARGS=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --compile-only) COMPILE_ONLY=1; shift ;;
        --run-only) RUN_ONLY=1; shift ;;
        --clean) CLEAN=1; shift ;;
        -s) EXTRA_ARGS="$EXTRA_ARGS -s"; shift ;;
        *) EXTRA_ARGS="$EXTRA_ARGS $1"; shift ;;
    esac
done

if [ $CLEAN -eq 1 ]; then
    rm -rf build
    rm -f plugins.cpp
    echo "Cleaned."
    [ $COMPILE_ONLY -eq 1 ] && exit 0
fi

mkdir -p build
cd build
cmake .. || exit 1
make optimization collect_plugins_exe || exit 1
cd ..

[ $COMPILE_ONLY -eq 1 ] && exit 0

if [ $RUN_ONLY -eq 0 ]; then
    # Собрать список .cpp файлов, переданных после -f и -m
    PLUGIN_FILES=""
    CAPTURE=0
    for arg in $EXTRA_ARGS; do
        if [[ "$arg" == "-f" || "$arg" == "-m" ]]; then
            CAPTURE=1
            continue
        fi
        if [ $CAPTURE -eq 1 ]; then
            # Проверяем, что это не ключ
            if [[ "$arg" != -* ]]; then
                PLUGIN_FILES="$PLUGIN_FILES $arg"
            else
                CAPTURE=0
            fi
        fi
    done

    if [ -n "$PLUGIN_FILES" ]; then
        build/collect_plugins_exe $PLUGIN_FILES
        cd build
        make optimization || exit 1
        cd ..
    fi
fi

sudo ./optimization $EXTRA_ARGS
