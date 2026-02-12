cmake_minimum_required(VERSION 3.10.0)
project(raycast VERSION 0.1.0 LANGUAGES C CXX)

find_package(SDL2 REQUIRED)

add_executable(raycast main.cpp)

target_link_libraries(raycast PRIVATE SDL2::SDL2)

