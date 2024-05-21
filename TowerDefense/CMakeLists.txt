cmake_minimum_required(VERSION 3.27)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_LIB "/Library/Frameworks/SDL2.framework/SDL2")
set(SDL2_IMAGE_LIB "/Library/Frameworks/SDL2_image.framework/SDL2_image")

find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        Application.h
        Application.cpp
        GameObject.h
        GameObject.cpp
        Component.h
        Component.cpp
        Tile.h
        Tile.cpp
        TileGrid.h
        TileGrid.cpp
        Timer.h
        Timer.cpp
        Vector2.h
        Vector2.cpp
        Scene.h
        Scene.h
        Scene.cpp
        GameplayScene.h
        GameplayScene.cpp
        SceneManager.h
        SceneManager.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "/Library/Frameworks/SDL2.framework")
target_include_directories(${PROJECT_NAME} PUBLIC "/Library/Frameworks/SDL2_image.framework")

target_link_libraries(${PROJECT_NAME} ${SDL2_LIB})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIB})
