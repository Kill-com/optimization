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
    optimization.exe -f plugins/f_list.cpp -m plugins/gold_sech.cpp plugins/porabola.cpp %EXTRA_ARGS%
    pause
) else (
    mkdir build
    cd build
    cmake ..
    @REM cmake --build build
    cd ..
)

