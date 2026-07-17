#!/bin/bash

# Проверяем флаг -f
if [[ "$1" == "-f" ]]; then
    mkdir -p build
    cd build || exit 1
    cmake ..
    make
    cd ..
fi

# Устанавливаем кодировку (для Linux не нужно chcp)
# Вместо optimization.exe используем ./optimization (или просто optimization)

./optimization -f target_function/function_comp/ -m methods/methods_comp/