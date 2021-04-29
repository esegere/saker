cmake_minimum_required(VERSION 3.16)
project(prompt)

set(CMAKE_CXX_STANDARD 17)

add_executable(saker-prompt main.cpp)

add_subdirectory(include)
include_directories(include)

target_link_libraries(saker-prompt saker)

