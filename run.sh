#!/bin/bash

# Инициализация переменных
COMPILE_ONLY=0
RUN_ONLY=0
CLEAN=0
EXTRA_ARGS=()
PLUGIN_FILES=""

# Функция для обработки аргументов
parse_args() {
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
                EXTRA_ARGS+=("-s")
                shift
                ;;
            *)
                EXTRA_ARGS+=("$1")
                shift
                ;;
        esac
    done
}

# Функция для обработки плагинов
process_plugins() {
    local capture=0
    
    for arg in "${EXTRA_ARGS[@]}"; do
        # Проверяем, является ли аргумент флагом
        if [[ "$arg" == -* ]]; then
            # Это флаг
            case "$arg" in
                -f|-m)
                    capture=1
                    ;;
                -s)
                    capture=0
                    ;;
            esac
        else
            # Это значение
            if [[ $capture -eq 1 ]]; then
                if [[ -n "$PLUGIN_FILES" ]]; then
                    PLUGIN_FILES="$PLUGIN_FILES $arg"
                else
                    PLUGIN_FILES="$arg"
                fi
            fi
        fi
    done
    
    echo "PLUGIN_FILES = $PLUGIN_FILES"
}

# Функция для определения правильного генератора CMake
get_cmake_generator() {
    # Проверяем, доступен ли MinGW Makefiles
    if cmake --help | grep -q "MinGW Makefiles"; then
        echo "MinGW Makefiles"
    else
        # По умолчанию используем Unix Makefiles
        echo "Unix Makefiles"
    fi
}

# Главная функция
main() {
    # Парсим аргументы
    parse_args "$@"
    
    # Определяем имя исполняемого файла
    if [[ "$(uname)" == "MINGW"* ]] || [[ "$(uname)" == "CYGWIN"* ]]; then
        EXE_NAME="optimization.exe"
    else
        EXE_NAME="optimization"
    fi
    
    # Обработка --clean
    if [[ $CLEAN -eq 1 ]]; then
        if [[ -d "build" ]]; then
            rm -rf build
        fi
        if [[ -f "plugins.cpp" ]]; then
            rm plugins.cpp
        fi
        echo "Cleaned."
        if [[ $COMPILE_ONLY -eq 1 ]]; then
            exit 0
        fi
    fi
    
    # Если есть ключ --run-only, пропускаем перегенерацию и пересборку
    if [[ $RUN_ONLY -eq 0 ]]; then
        # Обрабатываем плагины
        process_plugins
        
        # Запускаем make
        if [[ -n "$PLUGIN_FILES" ]]; then
            if [[ -d "support_by_make" ]]; then
                echo "Running make with ARGS=\"$PLUGIN_FILES\""
                cd support_by_make || exit 1
                make "ARGS=$PLUGIN_FILES"
                cd ..
            fi
        fi
        
        # Создаем build директорию
        mkdir -p build
        
        # Переходим в build
        cd build || exit 1
        
        # Определяем генератор
        GENERATOR=$(get_cmake_generator)
        echo "Using CMake generator: $GENERATOR"
        
        # Собираем основную программу и генератор
        cmake -G "$GENERATOR" .. || exit 1
        cmake --build .
        
        if [[ $? -ne 0 ]]; then
            cd ..
            exit $?
        fi
        
        cd ..
    fi
    
    # Если нужна только компиляция, выходим
    if [[ $COMPILE_ONLY -eq 1 ]]; then
        exit 0
    fi
    
    # Запуск основной программы
    if [[ -f "build/$EXE_NAME" ]]; then
        sudo ./build/$EXE_NAME "${EXTRA_ARGS[@]}"
    elif [[ -f "$EXE_NAME" ]]; then
        sudo ./$EXE_NAME "${EXTRA_ARGS[@]}"
    else
        echo "Error: $EXE_NAME not found in build/ or current directory"
        exit 1
    fi
    
    read -p "Press Enter to continue..."
}

# Запускаем главную функцию с переданными аргументами
main "$@"