cmake_minimum_required(VERSION 3.7)
project(TileEditor)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_LIB "/Library/Frameworks/SDL2.framework/SDL2")
set(SDL2_IMAGE_LIB "/Library/Frameworks/SDL2_image.framework/SDL2_image")

find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED)

include_directories(include/MGUI)
include_directories(${PROJECT_NAME} "/Library/Frameworks/SDL2.framework")
include_directories(${PROJECT_NAME} "/Library/Frameworks/SDL2_image.framework")

add_executable(
        ${PROJECT_NAME}
        main.cpp
        include/MGUI/UIState.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIB})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIB})

