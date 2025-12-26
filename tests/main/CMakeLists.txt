cmake_minimum_required(VERSION 3.30)
project(chutil_test_main VERSION 0.0.1 LANGUAGES CXX)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE chutil)

add_executable(test test_executable.cpp)
