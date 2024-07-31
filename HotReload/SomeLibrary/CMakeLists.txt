cmake_minimum_required(VERSION 3.27)
project(SomeLib)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} SHARED SomeLib.cpp include/SomeLib.h)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
