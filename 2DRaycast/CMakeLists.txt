cmake_minimum_required(VERSION 3.7)
project(2DRaycast)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_LIB "/Library/Frameworks/SDL2.framework/SDL2")

find_package(SDL2 REQUIRED COMPONENTS SDL2)

include_directories(2DRaycast "/Library/Frameworks/SDL2.framework")

add_executable(
        2DRaycast main.cpp)

target_link_libraries(2DRaycast ${SDL2_LIB})

