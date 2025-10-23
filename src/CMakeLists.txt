cmake_minimum_required(VERSION 3.10.0)

add_library(crashlog STATIC
    crashlog.cpp
    address.cpp
    win64/parser.cpp
    win64/stacktrace.cpp
    win64/symbols.cpp
    win64/exception.cpp
)

target_include_directories(crashlog PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../includes>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS crashlog
    EXPORT crashlogTargets
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../includes/
    DESTINATION include
)
