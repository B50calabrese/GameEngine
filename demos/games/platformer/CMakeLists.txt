add_executable(Platformer "main.cpp;gameplay_scene.cpp;level_complete_scene.cpp;level_loader.cpp")
target_link_libraries(Platformer PRIVATE GameEngine DemosCommon)
target_compile_definitions(Platformer PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")
set_target_properties(Platformer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/Platformer")
