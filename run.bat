@echo off
setlocal enabledelayedexpansion

set COMPILE_ONLY=0
set RUN_ONLY=0
set CLEAN=0
set EXTRA_ARGS=

:parse_args
if "%1"=="--compile-only" set COMPILE_ONLY=1 & shift & goto parse_args
if "%1"=="--run-only" set RUN_ONLY=1 & shift & goto parse_args
if "%1"=="--clean" set CLEAN=1 & shift & goto parse_args
if "%1"=="-s" set EXTRA_ARGS=%EXTRA_ARGS% -s & shift & goto parse_args
if "%1"=="" goto end_parse
set EXTRA_ARGS=%EXTRA_ARGS% %1
shift
goto parse_args
:end_parse

if %CLEAN%==1 (
    if exist build rmdir /s /q build
    if exist plugins.cpp del plugins.cpp
    echo Cleaned.
    if %COMPILE_ONLY%==1 exit /b 0
)

if not exist build mkdir build
cd build

:: Собираем основную программу и генератор
cmake .. || exit /b 1
cmake --build . --target optimization collect_plugins_exe || exit /b 1

cd ..

:: Если нужна только компиляция, выходим
if %COMPILE_ONLY%==1 exit /b 0

:: Если есть ключ --run-only, пропускаем перегенерацию plugins.cpp и пересборку
if %RUN_ONLY%==0 (
    :: Собираем список файлов плагинов из аргументов -f и -m
    set PLUGIN_FILES=
    :: Парсим EXTRA_ARGS, чтобы извлечь пути после -f и -m
    :: Упрощённо: считаем, что все аргументы после -f или -m до следующего ключа — это файлы
    :: Здесь нужна более аккуратная обработка, но для примера:
    for %%i in (%EXTRA_ARGS%) do (
        if "%%i"=="-f" set "CAPTURE=1"
        if "%%i"=="-m" set "CAPTURE=1"
        if not "%%i"=="-f" if not "%%i"=="-m" if "!CAPTURE!"=="1" (
            set PLUGIN_FILES=!PLUGIN_FILES! %%i
        )
    )
    :: Запускаем генератор
    if not "!PLUGIN_FILES!"=="" (
        build\collect_plugins_exe.exe !PLUGIN_FILES!
        :: Пересобираем основной проект с обновлённым plugins.cpp
        cd build
        cmake --build . --target optimization || exit /b 1
        cd ..
    )
)

:: Запуск основной программы
optimization.exe %EXTRA_ARGS%
pause
