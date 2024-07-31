cmake_minimum_required(VERSION 3.27)
project(MGUI)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_LIB       "/Library/Frameworks/SDL2.framework/SDL2")
set(SDL2_IMAGE_LIB "/Library/Frameworks/SDL2_image.framework/SDL2_image")
set(SDL2_TTF_LIB   "/Library/Frameworks/SDL2_ttf.framework/SDL2_ttf")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SDL2       REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf   REQUIRED)

file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES include/*.h)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PUBLIC "/Library/Frameworks/SDL2.framework")
target_include_directories(${PROJECT_NAME} PUBLIC "/Library/Frameworks/SDL2_image.framework")
target_include_directories(${PROJECT_NAME} PUBLIC "/Library/Frameworks/SDL2_ttf.framework")

target_link_libraries(${PROJECT_NAME} ${SDL2_LIB})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIB})
target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIB})
