cmake_minimum_required(VERSION 2.8)
project (scratchpad)

set(SCRATCHPAD_PRODUCTION FALSE CACHE BOOL "optimized")

set(cmake_module_path "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

if(CMAKE_COMPILER_IS_GNUCXX) # GNU Compiler
    if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    endif()
endif()

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
    set(TARGET_ARCHITECTURE "x64")
endif()

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY TRUE)

add_subdirectory(libraries/)
add_subdirectory(applications/)