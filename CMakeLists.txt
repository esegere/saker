cmake_minimum_required(VERSION 3.16)
project(prompt)

set(CMAKE_CXX_STANDARD 17)

find_package(FunctionalPlus REQUIRED)

add_executable(saker-prompt main.cpp userdata.hpp icons.hpp argh.hpp)

add_subdirectory(include)
include_directories(include)

target_link_libraries(saker-prompt saker FunctionalPlus::fplus git2)

install(
        TARGETS saker-prompt
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
