add_executable(LightingDemo main.cpp)
target_link_libraries(LightingDemo PRIVATE GameEngine)

# All demos use the shared assets directory
target_compile_definitions(LightingDemo PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")

# Set a consistent output directory for demos
set_target_properties(LightingDemo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/LightingDemo"
)
