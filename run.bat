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
:: Если есть ключ --run-only, пропускаем перегенерацию plugins.cpp и пересборку
if %RUN_ONLY%==0 (
    set CAPTURE=0
    
    for %%i in (%EXTRA_ARGS%) do (
        :: Проверяем, является ли аргумент флагом
        set "FIRST_CHAR=%%i"
        set "FIRST_CHAR=!FIRST_CHAR:~0,1!"
        
        if "!FIRST_CHAR!"=="-" (
            :: Это флаг
            if "%%i"=="-f" set "CAPTURE=1"
            if "%%i"=="-m" set "CAPTURE=1"
            if "%%i"=="-s" set "CAPTURE=0"
        ) else if "!CAPTURE!"=="1" (
            :: Это файл - добавляем в список
            if not "!PLUGIN_FILES!"=="" set PLUGIN_FILES=!PLUGIN_FILES! 
            set PLUGIN_FILES=!PLUGIN_FILES!%%i
        )
    )
    
    :: Выводим результат
    echo PLUGIN_FILES = !PLUGIN_FILES!
    
    :: Запускаем make
    if not "!PLUGIN_FILES!"=="" (
        if exist support_by_make (
            echo Running make with ARGS="!PLUGIN_FILES!"
            cd support_by_make
            make "ARGS=!PLUGIN_FILES!"
            cd ..
        )
    )
    if not exist build mkdir build
    cd build

    :: Собираем основную программу и генератор
    cmake -G "MinGW Makefiles" .. || exit /b 1
    cmake --build .

    if !ERRORLEVEL! neq 0 (
        exit /b %ERRORLEVEL%
        cd ..
    )
    cd ..
)

:: Если нужна только компиляция, выходим
if %COMPILE_ONLY%==1 exit /b 0


:: Запуск основной программы
optimization.exe %EXTRA_ARGS%
pause
