# GetStarted 最小示例。三种用法：
#   1. 随源码根工程构建（根 CMakeLists add_subdirectory）
#   2. 源码仓库内独立构建（cmake -S examples/GetStarted -B build）
#   3. dist 预编译包内独立构建（链接 lib/<os>-<arch>/ 下的静态库）
cmake_minimum_required(VERSION 3.20)
project(GetStarted CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT TARGET thingboot_device)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../CMakeLists.txt")
        # 源码仓库内：随根工程构建
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../..
            ${CMAKE_BINARY_DIR}/thingboot-sdk)
    else ()
        # dist 包内：导入预编译静态库
        file(GLOB_RECURSE THINGBOOT_LIB
            "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/*/thingboot_device.lib"
            "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/*/libthingboot_device.a")
        if (NOT THINGBOOT_LIB)
            message(FATAL_ERROR "thingboot_device 预编译库未找到（lib/<os>-<arch>/）")
        endif ()
        list(GET THINGBOOT_LIB 0 THINGBOOT_LIB)
        message(STATUS "using prebuilt thingboot_device: ${THINGBOOT_LIB}")
        add_library(thingboot_device STATIC IMPORTED)
        set_target_properties(thingboot_device PROPERTIES
            IMPORTED_LOCATION "${THINGBOOT_LIB}"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
        )
        if (WIN32)
            target_link_libraries(thingboot_device INTERFACE ws2_32 advapi32)
            if (MSVC)
                target_compile_options(thingboot_device INTERFACE /utf-8)
            endif ()
        endif ()
    endif ()
endif ()

add_executable(get_started main.cpp)
target_link_libraries(get_started PRIVATE thingboot_device)
