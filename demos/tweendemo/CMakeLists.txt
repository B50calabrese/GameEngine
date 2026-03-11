set(DEMO_NAME TweenDemo)

# Create the executable for the demo
add_executable(${DEMO_NAME} main.cpp)

# Link the GameEngine library to the demo executable
target_link_libraries(${DEMO_NAME} PRIVATE GameEngine)

# Set the assets path for the demo
set(ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/")
target_compile_definitions(${DEMO_NAME} PRIVATE ENGINE_ASSETS_PATH="${ASSETS_PATH}")

# Set the output directory for the demo executable
set_target_properties(${DEMO_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/tweendemo"
)
