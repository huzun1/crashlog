cmake_minimum_required(VERSION 3.10.0)

add_library(crashlog STATIC
    exception_info.cpp
    exception_info.h
    internal.cpp
    internal.h
    parser.cpp
    parser.h
)

target_include_directories(crashlog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})