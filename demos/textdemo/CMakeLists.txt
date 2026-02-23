add_executable(TextDemo main.cpp)

# Sets the asset directory.
set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

file(COPY "${ASSET_SOURCE_DIR}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_definitions(-DENGINE_ASSETS_PATH="${ASSET_BUILD_DIR}/")

target_link_libraries(TextDemo PRIVATE GameEngine)
