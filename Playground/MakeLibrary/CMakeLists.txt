cmake_minimum_required(VERSION 3.27)
project(MakeLibrary)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Engine)
add_subdirectory(EngineEditor)
add_subdirectory(GameProject)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC Engine/include)

target_link_libraries(${PROJECT_NAME} M_ENGINE)
target_link_libraries(${PROJECT_NAME} M_ENGINE_EDITOR)


