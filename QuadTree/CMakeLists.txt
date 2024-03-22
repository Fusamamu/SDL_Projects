cmake_minimum_required(VERSION 3.7)
project(QuadTree)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_LIB "/Library/Frameworks/SDL2.framework/SDL2")

find_package(SDL2 REQUIRED COMPONENTS SDL2)

include_directories(QuadTree "include")
include_directories(QuadTree "/Library/Frameworks/SDL2.framework")

add_executable(
        QuadTree main.cpp
        include/QuadTree.h
        include/BBox.h
        include/Point.h
        Object.cpp
        Object.h)

target_link_libraries(QuadTree ${SDL2_LIB})

