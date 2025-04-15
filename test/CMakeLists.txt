cmake_minimum_required(VERSION 3.10.0)
project(crashlog_test CXX)

add_executable(crashlog_test main.cpp)
target_link_libraries(crashlog_test crashlog)