cmake_minimum_required(VERSION 3.10.0)

add_library(crashlog STATIC
    exception_info.cpp
    internal.cpp
    parser.cpp
)

target_include_directories(crashlog PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS crashlog
    EXPORT crashlogTargets
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/includes/
    DESTINATION include
)