cmake_minimum_required(VERSION 3.27)
project(Physics)
set(CMAKE_CXX_STANDARD 17)

set(SDL2_LIB "/Library/Frameworks/SDL2.framework/SDL2")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED COMPONENTS SDL2)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PUBLIC "/Library/Frameworks/SDL2.framework")

target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIB})
