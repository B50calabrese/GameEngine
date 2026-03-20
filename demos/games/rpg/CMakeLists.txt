add_executable(RpgDemo "main.cpp")
target_link_libraries(RpgDemo PRIVATE GameEngine DemosCommon)
target_compile_definitions(RpgDemo PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")
set_target_properties(RpgDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/RpgDemo")
