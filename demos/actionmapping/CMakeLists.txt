# Set a variable for the demo name
set(DEMO_NAME ActionMappingDemo)

# Find all source files
file(GLOB SOURCE_FILES "*.cpp")

# Create the executable
add_executable(${DEMO_NAME} ${SOURCE_FILES})

# Add the assets path as a compile definition
target_compile_definitions(${DEMO_NAME} PRIVATE
    ENGINE_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)

# Link against the GameEngine library
target_link_libraries(${DEMO_NAME} PRIVATE GameEngine)

# Set the output directory to be consistent with other demos
set_target_properties(${DEMO_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/actionmapping"
)
