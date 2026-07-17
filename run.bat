@echo off
if "%~1"=="-f" (
    mkdir build
    cd build
    cmake ..
    make
    cd ..
)

chcp 65001 > nul
optimization.exe -f target_function/function_comp/ -m methods/methods_comp/
pause