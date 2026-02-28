cmake_minimum_required(VERSION 3.10)
project(GameEngine VERSION 1.0)

# 1. Configuration
# ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 2. Define Paths using CURRENT_SOURCE_DIR
# ----------------------------------------
# This ensures paths work even when this project is included via add_subdirectory
set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(THIRD_PARTY_DIR "${ENGINE_ROOT}/third_party")
set(THIRD_PARTY_INCLUDE "${THIRD_PARTY_DIR}/include")
set(THIRD_PARTY_LIBS "${THIRD_PARTY_DIR}/libs")

# Inform the linker where to find the library files (e.g., glfw3.lib)
link_directories("${THIRD_PARTY_LIBS}")

# 3. Sources
# ----------
# We prefix these with ENGINE_ROOT to ensure they are found correctly
set(SOURCE_FILES
    "${ENGINE_ROOT}/src/application.cpp"
    "${ENGINE_ROOT}/src/engine.cpp"
    "${ENGINE_ROOT}/src/input_manager.cpp"
    "${ENGINE_ROOT}/src/scene_manager.cpp"
    "${ENGINE_ROOT}/src/window.cpp"
    "${ENGINE_ROOT}/src/ecs/entity_manager.cpp"
    "${ENGINE_ROOT}/src/graphics/buffer_utils.cpp"
    "${ENGINE_ROOT}/src/graphics/camera.cpp"
    "${ENGINE_ROOT}/src/graphics/primitive_renderer.cpp"
    "${ENGINE_ROOT}/src/graphics/renderer.cpp"
    "${ENGINE_ROOT}/src/graphics/shader.cpp"
    "${ENGINE_ROOT}/src/graphics/text_renderer.cpp"
    "${ENGINE_ROOT}/src/graphics/texture.cpp"
    "${ENGINE_ROOT}/third_party/libs/glad.c"
)

# Use GLOB_RECURSE relative to CURRENT_SOURCE_DIR
file(GLOB_RECURSE INCLUDE_FILES
    "${ENGINE_ROOT}/include/*.h"
    "${ENGINE_ROOT}/include/ecs/*.h"
    "${ENGINE_ROOT}/include/graphics/*.h"
    "${ENGINE_ROOT}/src/*.h"
    "${ENGINE_ROOT}/src/ecs/*.h"
    "${ENGINE_ROOT}/src/graphics/*.h" 
)

# 4. Create Library
# -----------------
add_library(GameEngine STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

# 5. Linking Dependencies & Includes
# ----------------------------------
target_include_directories(GameEngine 
    PUBLIC  "${ENGINE_ROOT}/include"
    PRIVATE "${ENGINE_ROOT}/src"
    PUBLIC  "${THIRD_PARTY_INCLUDE}"
    PUBLIC  "${THIRD_PARTY_INCLUDE}/freetype"
)

# Link the GLFW library and all necessary system libraries.
if(WIN32)
    target_link_libraries(GameEngine PRIVATE
        "${THIRD_PARTY_LIBS}/glfw3.lib"
        "${THIRD_PARTY_LIBS}/freetype.lib"
        opengl32
        gdi32
        shell32
        user32
    )
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    pkg_check_modules(FREETYPE REQUIRED freetype2)
    target_link_libraries(GameEngine PUBLIC ${GLFW_LIBRARIES} ${FREETYPE_LIBRARIES} GL dl)
    target_include_directories(GameEngine PUBLIC ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
endif()

# --- 6. Add Demos (Optional when building as a library) ---
# We wrap this in a check so the game doesn't build engine demos by default
if(PROJECT_NAME STREQUAL "GameEngine")
    add_subdirectory(demos/applicationdemo)
    add_subdirectory(demos/ecsdemo)
    add_subdirectory(demos/rendererdemo)
    add_subdirectory(demos/scenedemo)
    add_subdirectory(demos/textdemo)
    add_subdirectory(demos/windowdemo)
endif()
