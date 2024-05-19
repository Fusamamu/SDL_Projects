cmake_minimum_required(VERSION 3.7)
project(Voronoi)

set(CMAKE_CXX_STANDARD 17)
set(SDL2_LIB "/Library/Frameworks/SDL2.framework/SDL2")

find_package(SDL2 REQUIRED COMPONENTS SDL2)

add_executable(${PROJECT_NAME}
        main.cpp
        Timer.cpp
        Application.cpp
        include/Application.h
        include/Timer.h
        include/Vector2.h
        Vector2.cpp
        include/VoronoiDiagram.h
        include/VoronoiDiagram.h
        VoronoiDiagram.cpp
        include/Beachline.h
        Beachline.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PUBLIC "/Library/Frameworks/SDL2.framework")
target_link_libraries(${PROJECT_NAME} ${SDL2_LIB})

