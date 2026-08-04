#!/bin/bash

# Инициализация переменных
COMPILE_ONLY=0
RUN_ONLY=0
CLEAN=0
EXTRA_ARGS=""
PLUGIN_FILES=""
CAPTURE=0

# Парсинг аргументов
while [[ $# -gt 0 ]]; do
    case "$1" in
        --compile-only)
            COMPILE_ONLY=1
            shift
            ;;
        --run-only)
            RUN_ONLY=1
            shift
            ;;
        --clean)
            CLEAN=1
            shift
            ;;
        -s)
            EXTRA_ARGS="$EXTRA_ARGS -s"
            shift
            ;;
        *)
            EXTRA_ARGS="$EXTRA_ARGS $1"
            shift
            ;;
    esac
done

# Очистка
if [ $CLEAN -eq 1 ]; then
    if [ -d "build" ]; then
        rm -rf build
    fi
    if [ -f "plugins.cpp" ]; then
        rm plugins.cpp
    fi
    echo "Cleaned."
    if [ $COMPILE_ONLY -eq 1 ]; then
        exit 0
    fi
fi

# Если есть ключ --run-only, пропускаем перегенерацию plugins.cpp и пересборку
if [ $RUN_ONLY -eq 0 ]; then
    CAPTURE=0
    PLUGIN_FILES=""
    
    # Парсим EXTRA_ARGS для поиска -f и -m
    for arg in $EXTRA_ARGS; do
        # Проверяем, является ли аргумент флагом
        if [[ "$arg" == -* ]]; then
            # Это флаг
            if [ "$arg" = "-f" ] || [ "$arg" = "-m" ]; then
                CAPTURE=1
            elif [ "$arg" = "-s" ]; then
                CAPTURE=0
            fi
        elif [ $CAPTURE -eq 1 ]; then
            # Это файл - добавляем в список
            if [ -n "$PLUGIN_FILES" ]; then
                PLUGIN_FILES="$PLUGIN_FILES "
            fi
            PLUGIN_FILES="$PLUGIN_FILES$arg"
        fi
    done
    
    # Выводим результат
    echo "PLUGIN_FILES = $PLUGIN_FILES"
    
    # Запускаем make
    if [ -n "$PLUGIN_FILES" ]; then
        if [ -d "support_by_make" ]; then
            echo "Running make with ARGS=\"$PLUGIN_FILES\""
            cd support_by_make || exit 1
            make "ARGS=$PLUGIN_FILES"
            cd ..
        fi
    fi
    
    # Создаём build директорию
    if [ ! -d "build" ]; then
        mkdir build
    fi
    
    # Собираем основную программу и генератор
    cd build || exit 1
    cmake -G "Unix Makefiles" .. || exit 1
    cmake --build .
    
    if [ $? -ne 0 ]; then
        cd ..
        exit 1
    fi
    cd ..
fi

# Если нужна только компиляция, выходим
if [ $COMPILE_ONLY -eq 1 ]; then
    exit 0
fi

# Запуск основной программы
./optimization.exe $EXTRA_ARGS

# Ожидание нажатия клавиши (аналог pause)
read -p "Press any key to continue..."