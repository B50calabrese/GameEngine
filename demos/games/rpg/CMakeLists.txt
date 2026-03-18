# 1. Project Setup
# ----------------
project(RpgDemo VERSION 1.0)

# 2. Add Executable
# -----------------
add_executable(Rpg main.cpp)

# 3. Include and Link Engine
# --------------------------
target_link_libraries(Rpg PRIVATE GameEngine)

# 4. Define Engine Assets Path for the Demo
# -----------------------------------------
target_compile_definitions(Rpg PRIVATE
    ENGINE_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets"
    COMMON_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../../common/assets/"
)

# 5. Copy Assets (if any)
# -----------------------
# (Assuming assets will be added later if needed)
# file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
