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
list(FILTER SOURCES EXCLUDE REGEX ".*/support_by_make/.*")

list(APPEND SOURCES ${ADDITIONAL_SOURCES})

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_target(build_tools
    COMMAND ${CMAKE_MAKE_PROGRAM} -f Makefile 
    SRC_DIR=../target_function/f_list 
    PLUGIN_DIR=../target_function/compiled/f_comp 
    COMMAND ${CMAKE_MAKE_PROGRAM} -f Makefile 
    SRC_DIR=../methods/methods_list 
    PLUGIN_DIR=../methods/compiled/methods_comp 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/support_by_make 
    COMMENT "Сборка external_tools с помощью Makefile"
    VERBATIM
)
# Делаем так, чтобы эти цели собирались вместе с основным проектом
add_dependencies(${PROJECT_NAME} build_tools)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_SOURCE_DIR}/
    COMMENT "Копирование ${PROJECT_NAME}.exe в главную папку"
)