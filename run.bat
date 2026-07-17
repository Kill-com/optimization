@echo off
chcp 65001 > nul
optimization.exe -f target_function/function_comp/ -m methods/methods_comp/
pause