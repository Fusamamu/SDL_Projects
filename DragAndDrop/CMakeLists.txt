cmake_minimum_required(VERSION 3.27)
project(DragAndDrop)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_LIB "/Library/Frameworks/SDL2.framework/SDL2")

find_package(SDL2 REQUIRED COMPONENTS SDL2)

include_directories(DragAndDrop "/Library/Frameworks/SDL2.framework")

add_executable(DragAndDrop main.cpp)

target_link_libraries(DragAndDrop ${SDL2_LIB})


