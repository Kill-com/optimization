# FindPAPI.cmake
find_path(PAPI_INCLUDE_DIRS 
    NAMES papi.h
    PATHS /usr/include /usr/local/include /opt/papi/include
)

find_library(PAPI_LIBRARIES
    NAMES papi
    PATHS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib /opt/papi/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PAPI DEFAULT_MSG PAPI_LIBRARIES PAPI_INCLUDE_DIRS)

mark_as_advanced(PAPI_INCLUDE_DIRS PAPI_LIBRARIES)