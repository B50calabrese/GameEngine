add_executable(Breakout "main.cpp")
target_link_libraries(Breakout PRIVATE GameEngine DemosCommon)
target_compile_definitions(Breakout PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")
set_target_properties(Breakout PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/Breakout")
