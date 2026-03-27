cmake_minimum_required(VERSION 3.10)
project(DemosCommon VERSION 1.0)

add_library(DemosCommon STATIC
    menu_scene.cpp
    menu_scene.h
    demo_utils.cpp
    demo_utils.h
)

target_link_libraries(DemosCommon PRIVATE GameEngine)
target_include_directories(DemosCommon PUBLIC . "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
