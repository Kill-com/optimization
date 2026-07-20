set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.13)
project(optimization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================
# Определение платформы
# ============================================
if(WIN32)
    message(STATUS " Building for Windows (MinGW)")
    set(IS_WINDOWS TRUE)
else()
    message(STATUS " Building for Linux")
    set(IS_WINDOWS FALSE)
endif()

# ============================================
# PAPI - только для Linux
# ============================================
option(USE_PAPI "Enable PAPI support" ON)

if(NOT WIN32 AND USE_PAPI)
    # Используем find_package для более надежного поиска
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
    find_package(PAPI)
    
    if(PAPI_FOUND)
        message(STATUS " PAPI found via find_package: ${PAPI_LIBRARIES}")
        set(HAVE_PAPI TRUE)
        set(PAPI_INCLUDE_DIR ${PAPI_INCLUDE_DIRS})
        set(PAPI_LIBRARY ${PAPI_LIBRARIES})
    else()
        # Ручной поиск, если find_package не сработал
        message(STATUS " PAPI not found via find_package, trying manual search...")
        find_path(PAPI_INCLUDE_DIR NAMES papi.h 
            PATHS /usr/include /usr/local/include /opt/papi/include)
        find_library(PAPI_LIBRARY NAMES papi 
            PATHS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib /opt/papi/lib)
        
        if(PAPI_INCLUDE_DIR AND PAPI_LIBRARY)
            message(STATUS " PAPI found manually: ${PAPI_LIBRARY}")
            set(HAVE_PAPI TRUE)
        else()
            message(WARNING " PAPI not found - using stub mode")
            set(HAVE_PAPI FALSE)
        endif()
    endif()
else()
    set(HAVE_PAPI FALSE)
    if(WIN32)
        message(STATUS " PAPI disabled on Windows")
    else()
        message(STATUS " PAPI disabled by user")
    endif()
endif()

# ============================================
# Исходники
# ============================================
file(GLOB_RECURSE SOURCES "./*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*/build/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*CMakeCXXCompilerId.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/methods/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/target_function/.*")
list(FILTER SOURCES EXCLUDE REGEX ".*/support_by_make/.*")

add_executable(${PROJECT_NAME} ${SOURCES})

# ============================================
# Подключение библиотек
# ============================================
if(HAVE_PAPI)
    target_include_directories(${PROJECT_NAME} PRIVATE ${PAPI_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${PAPI_LIBRARY})
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_PAPI)
    
    # Добавляем дополнительные библиотеки, если нужны
    # pfm_* функции находятся в libpapi, но могут требовать libpfm4
    find_library(PFM_LIBRARY NAMES pfm libpfm)
    if(PFM_LIBRARY)
        target_link_libraries(${PROJECT_NAME} ${PFM_LIBRARY})
        message(STATUS " PFM library found: ${PFM_LIBRARY}")
    endif()
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NO_PAPI)
endif()

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
add_dependencies(${PROJECT_NAME} build_tools)

# ============================================
# Копирование исполняемого файла
# ============================================
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:${PROJECT_NAME}>
        ${CMAKE_SOURCE_DIR}/
    COMMENT "Копирование ${PROJECT_NAME} в главную папку"
)