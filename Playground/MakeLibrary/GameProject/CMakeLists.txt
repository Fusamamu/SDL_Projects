cmake_minimum_required(VERSION 3.27)
project(Game)

add_executable(${PROJECT_NAME}
        Game.cpp include/Game.h)

target_include_directories(${PROJECT_NAME} PUBLIC include)
