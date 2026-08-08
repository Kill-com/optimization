set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.13)
project(optimization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# Исходники
# ============================================
file(GLOB_RECURSE SOURCES "./*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*CMakeCXXCompilerId.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/support_by_make/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/plugins/.*")

add_executable(${PROJECT_NAME} ${SOURCES})

# ============================================
# Поиск библиотеки PFM (обязательно)
# ============================================
find_package(PkgConfig REQUIRED)

# Ищем libpfm4 через pkg-config
pkg_check_modules(PFM libpfm4)
if(PFM_FOUND)
    message(STATUS "✅ PFM found via pkg-config")
    target_include_directories(${PROJECT_NAME} PRIVATE ${PFM_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PFM_LIBRARIES})
    target_compile_options(${PROJECT_NAME} PRIVATE ${PFM_CFLAGS_OTHER})
else()
    # Если pkg-config не нашел, ищем вручную
    find_library(PFM_LIBRARY NAMES pfm)
    find_path(PFM_INCLUDE_DIR perfmon/pfmlib.h)
    
    if(PFM_LIBRARY AND PFM_INCLUDE_DIR)
        message(STATUS "✅ PFM found manually")
        target_include_directories(${PROJECT_NAME} PRIVATE ${PFM_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PFM_LIBRARY})
    else()
        message(FATAL_ERROR "❌ PFM library not found! Install libpfm4-dev")
    endif()
endif()

# ============================================
# Опционально: PAPI (если нужен)
# ============================================
option(USE_PAPI "Enable PAPI support" OFF)

if(USE_PAPI)
    find_package(PAPI REQUIRED)
    if(PAPI_FOUND)
        message(STATUS "✅ PAPI found")
        target_include_directories(${PROJECT_NAME} PRIVATE ${PAPI_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${PAPI_LIBRARIES})
        target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_PAPI)
    else()
        message(STATUS "⚠️ PAPI not found, compiling without PAPI support")
        target_compile_definitions(${PROJECT_NAME} PRIVATE NO_PAPI)
    endif()
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NO_PAPI)
endif()

# ============================================
# Дополнительные флаги для Linux
# ============================================
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(${PROJECT_NAME} PRIVATE __linux__)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_DL_LIBS})
endif()

# ============================================
# Флаги компиляции для отладки (опционально)
# ============================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall -Wextra)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O2 -Wall)
endif()

# ============================================
# Копирование исполняемого файла
# ============================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_SOURCE_DIR}/
    COMMENT "Копирование ${PROJECT_NAME} в главную папку"
)

# ============================================
# Вывод информации о сборке
# ============================================
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Sources: ${SOURCES}")
message(STATUS "========================================")