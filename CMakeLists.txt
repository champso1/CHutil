cmake_minimum_required(VERSION 3.30)
project(chutil VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(sources
  src/chutil.cpp
)
set(headers
  include/chutil/chutil.hpp)
add_library(chutil STATIC)
target_sources(chutil PUBLIC ${sources})
target_sources(chutil PUBLIC
  FILE_SET cxx_headers
  TYPE HEADERS
  FILES ${headers}
)
target_include_directories(chutil PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_options(chutil PUBLIC -Wall -Wextra -Wpedantic)
add_subdirectory(tests)
