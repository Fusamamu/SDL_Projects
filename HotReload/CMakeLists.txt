cmake_minimum_required(VERSION 3.27)
project(HotReload)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(SomeLibrary)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "/SomeLibrary/include")

target_link_libraries(${PROJECT_NAME} PUBLIC SomeLib)
