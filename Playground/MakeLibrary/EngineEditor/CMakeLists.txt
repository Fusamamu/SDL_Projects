add_library(M_ENGINE_EDITOR STATIC EngineEditor.cpp)

target_include_directories(M_ENGINE_EDITOR PUBLIC include)

target_link_libraries(M_ENGINE_EDITOR PUBLIC M_ENGINE)