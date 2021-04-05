cmake_minimum_required(VERSION 3.19)
project(prompt)

set(CMAKE_CXX_STANDARD 17)

add_executable(prompt main.cpp)

add_subdirectory(include)
include_directories(include)

target_link_libraries(prompt saker)

