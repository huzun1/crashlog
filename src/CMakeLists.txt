cmake_minimum_required(VERSION 3.10.0)

add_library(crashlog STATIC
    exception_info.cpp
    exception_info.h
    internal.cpp
    internal.h
    parser.cpp
    parser.h
)

target_include_directories(crashlog PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS crashlog
    EXPORT crashlogTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
install(EXPORT crashlogTargets
    FILE crashlogTargets.cmake
    NAMESPACE crashlog::
    DESTINATION lib/cmake/crashlog
)