cmake_minimum_required(VERSION 3.10.0)
project(raycast VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

# Collect all source files under src/ recursively so new files are linked automatically
file(GLOB_RECURSE RAYCAST_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.c)
list(APPEND RAYCAST_SOURCES main.cpp)

add_executable(raycast ${RAYCAST_SOURCES})

target_include_directories(raycast PRIVATE include)

target_link_libraries(raycast PRIVATE SDL2::SDL2)

