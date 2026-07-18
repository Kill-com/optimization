@echo off
setlocal enabledelayedexpansion

REM ============================================
REM ПРОВЕРКА ПРАВ АДМИНИСТРАТОРА
REM ============================================
net session > nul 2>&1
if errorlevel 1 (
    echo [ОШИБКА] Требуются права администратора!
    echo Запустите скрипт от имени администратора.
    echo.
    echo Чтобы запустить от имени администратора:
    echo   - Нажмите правой кнопкой на run.bat
    echo   - Выберите "Запуск от имени администратора"
    pause
    exit /b 1
)

REM ============================================
REM ПРОВЕРКА CHOCOLATEY
REM ============================================
echo Проверка Chocolatey...

choco -v > nul 2>&1

if errorlevel 1 (
    echo [ОШИБКА] Chocolatey не установлен!
    echo Установка Chocolatey...
    echo.
    
    powershell -NoProfile -ExecutionPolicy Bypass -Command "iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))"
    
    if errorlevel 1 (
        echo [ОШИБКА] Не удалось установить Chocolatey!
        echo Попробуйте установить вручную: https://chocolatey.org/install
        pause
        exit /b 1
    )
    
    echo [УСПЕШНО] Chocolatey установлен!
    echo.
    
    REM ✅ Обновляем PATH для текущей сессии
    for /f "delims=" %%i in ('choco -v 2^>nul') do set CHOCO_VER=%%i
    if defined CHOCO_VER (
        echo Chocolatey уже доступен в PATH
    ) else (
        echo Обновляем PATH...
        set "PATH=%ProgramData%\chocolatey\bin;%PATH%"
        echo Добавлен путь: %ProgramData%\chocolatey\bin
    )
    echo.
    echo Версия: 
    choco -v
    echo.
    echo [УСПЕШНО] Chocolatey готов к использованию!
)

echo [УСПЕШНО] Chocolatey найден!

REM ============================================
REM ПРОВЕРКА CMAKE
REM ============================================
echo Проверка CMake...

cmake --version > nul 2>&1

if errorlevel 1 (
    echo [ОШИБКА] CMake не установлен!
    echo Установка CMake через Chocolatey...
    echo.
    
    choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System' -y
    
    if errorlevel 1 (
        echo [ОШИБКА] Не удалось установить CMake!
        echo Попробуйте установить вручную: https://cmake.org/download/
        pause
        exit /b 1
    )
    
    echo [УСПЕШНО] CMake установлен!
    echo.
    
    REM ✅ Обновляем PATH для текущей сессии
    where cmake > nul 2>&1
    if errorlevel 1 (
        echo Обновляем PATH для CMake...
        for /f "delims=" %%i in ('dir /s /b "C:\Program Files\CMake\bin\cmake.exe" 2^>nul') do (
            set "CMAKE_PATH=%%~dpi"
            set "PATH=%%~dpi;%PATH%"
            echo Добавлен путь: %%~dpi
        )
    )
    echo.
    echo Версия:
    cmake --version
    echo.
    echo [УСПЕШНО] CMake готов к использованию!
)

echo [УСПЕШНО] CMake найден!

REM ============================================
REM СБОРКА ПРОЕКТА
REM ============================================
if "%~1"=="-f" (
    echo.
    echo Сборка проекта...
    
    if not exist build mkdir build
    
    cd build || exit /b 1
    
    echo Запуск CMake...
    cmake ..
    if errorlevel 1 (
        echo [ОШИБКА] Ошибка CMake!
        echo.
        echo Проверьте наличие CMakeLists.txt
        pause
        exit /b 1
    )
    
    echo Сборка...
    make
    if errorlevel 1 (
        echo [ОШИБКА] Ошибка сборки!
        echo.
        echo Проверьте наличие ошибок компиляции
        pause
        exit /b 1
    )
    
    cd ..
    echo [УСПЕШНО] Сборка завершена!
)

REM ============================================
REM ПОИСК ИСПОЛНЯЕМОГО ФАЙЛА
REM ============================================
echo.
echo Поиск optimization.exe...

set "EXE_PATH="

REM Проверяем в корневой папке
if exist ".\optimization.exe" (
    set "EXE_PATH=.\optimization.exe"
    echo Найден в корневой папке
)

REM Проверяем в папке build
if exist ".\build\optimization.exe" (
    set "EXE_PATH=.\build\optimization.exe"
    echo Найден в папке build
)

REM Проверяем в папке build/Release
if exist ".\build\Release\optimization.exe" (
    set "EXE_PATH=.\build\Release\optimization.exe"
    echo Найден в папке build/Release
)

REM Проверяем в папке build/Debug
if exist ".\build\Debug\optimization.exe" (
    set "EXE_PATH=.\build\Debug\optimization.exe"
    echo Найден в папке build/Debug
)

if not defined EXE_PATH (
    echo [ОШИБКА] optimization.exe не найден!
    echo.
    echo Проверьте следующие папки:
    echo   - .\
    echo   - .\build\
    echo   - .\build\Release\
    echo   - .\build\Debug\
    echo.
    echo Попробуйте собрать проект: run.bat -f
    pause
    exit /b 1
)

REM ============================================
REM ЗАПУСК ПРОГРАММЫ
REM ============================================
echo.
echo Запуск оптимизации...

chcp 65001 > nul

echo Исполняемый файл: %EXE_PATH%
echo.
"%EXE_PATH%" -f target_function/function_comp/ -m methods/methods_comp/

if errorlevel 1 (
    echo [ОШИБКА] Программа завершилась с ошибкой!
    echo Код ошибки: %errorlevel%
    pause
    exit /b 1
)

echo.
echo [УСПЕШНО] Программа завершена!
pause