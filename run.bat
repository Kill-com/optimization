@echo off

set EXTRA_ARGS=

:parse_args
if "%1"=="-s" (
    set EXTRA_ARGS=%EXTRA_ARGS% -s
    shift
    goto parse_args
)

if exist "build" (
    chcp 65001 > nul
    optimization.exe -f target_function/compiled/f_comp/ -m methods/compiled/methods_comp/ %EXTRA_ARGS%
    pause
) else (
    mkdir build
    cd build
    cmake ..
    make
    cd ..
)

