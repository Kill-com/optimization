cmake_minimum_required(VERSION 3.13)
project(optimization)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим все .cpp файлы
file(GLOB_RECURSE SOURCES "./*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*CMakeCXXCompilerId.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/methods/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/target_function/.*")

list(APPEND SOURCES ${ADDITIONAL_SOURCES})

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_target(build_external_tools
    COMMAND ${CMAKE_MAKE_PROGRAM} -f Makefile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/methods
    COMMENT "Сборка external_tools с помощью Makefile"
    VERBATIM
)
add_custom_target(build_function_tools
    COMMAND ${CMAKE_MAKE_PROGRAM} -f Makefile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/target_function
    COMMENT "Сборка external_tools с помощью Makefile"
    VERBATIM
)

# Делаем так, чтобы эти цели собирались вместе с основным проектом
add_dependencies(${PROJECT_NAME} build_external_tools build_function_tools)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_SOURCE_DIR}/
    COMMENT "Копирование ${PROJECT_NAME}.exe в главную папку"
)