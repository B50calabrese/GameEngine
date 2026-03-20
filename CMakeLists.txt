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
    "${ENGINE_ROOT}/src/engine/core/application.cpp"
    "${ENGINE_ROOT}/src/engine/core/engine.cpp"
    "${ENGINE_ROOT}/src/engine/core/job_system.cpp"
    "${ENGINE_ROOT}/src/engine/core/window.cpp"
    "${ENGINE_ROOT}/src/engine/input/input_manager.cpp"
    "${ENGINE_ROOT}/src/engine/input/action_manager.cpp"
    "${ENGINE_ROOT}/src/engine/physics/physics_system.cpp"
    "${ENGINE_ROOT}/src/engine/ui/ui_systems.cpp"
    "${ENGINE_ROOT}/src/engine/scene/scene_manager.cpp"
    "${ENGINE_ROOT}/src/engine/util/logger.cpp"
    "${ENGINE_ROOT}/src/engine/util/easing.cpp"
    "${ENGINE_ROOT}/src/engine/util/tween_manager.cpp"
    "${ENGINE_ROOT}/src/engine/ecs/entity_manager.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/buffer_utils.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/camera.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/font.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/framebuffer.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/particle_system.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/post_processor.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/primitive_renderer.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/renderer.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/shader.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/sprite_animator.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/sprite_render_system.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/sprite_sheet.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/text_renderer.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/texture.cpp"
    "${ENGINE_ROOT}/third_party/libs/glad.c"
)

# Use GLOB_RECURSE relative to CURRENT_SOURCE_DIR
file(GLOB_RECURSE INCLUDE_FILES
    "${ENGINE_ROOT}/include/engine/*.h"
    "${ENGINE_ROOT}/src/engine/*.h"
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
    # Common assets for all demos
    set(COMMON_ASSETS_DIR "${ENGINE_ROOT}/demos/assets")
    set(COMMON_ASSETS_BUILD_DIR "${CMAKE_BINARY_DIR}/demos/assets")

    # Copy common assets once to the build directory
    file(COPY "${COMMON_ASSETS_DIR}/" DESTINATION "${COMMON_ASSETS_BUILD_DIR}")

    # Helper function to add a demo executable
    function(add_engine_demo DEMO_NAME SOURCE_FILES)
        add_executable(${DEMO_NAME} ${SOURCE_FILES})
        target_link_libraries(${DEMO_NAME} PRIVATE GameEngine)

        # All demos use the shared assets directory
        target_compile_definitions(${DEMO_NAME} PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")

        # Set a consistent output directory for demos
        set_target_properties(${DEMO_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/${DEMO_NAME}"
        )
    endfunction()

    add_subdirectory(demos/applicationdemo)
    add_subdirectory(demos/ecsdemo)
    add_subdirectory(demos/rendererdemo)
    add_subdirectory(demos/renderqueuedemo)
    add_subdirectory(demos/scenedemo)
    add_subdirectory(demos/textdemo)
    add_subdirectory(demos/windowdemo)
    add_subdirectory(demos/actionmapping)
    add_subdirectory(demos/tweendemo)
    add_subdirectory(demos/spritesheetdemo)
    add_subdirectory(demos/animatordemo)
    add_subdirectory(demos/post_effects)
    add_subdirectory(demos/jobsystemdemo)
    add_subdirectory(demos/ui_menu)
    add_subdirectory(demos/common)
    add_subdirectory(demos/games)
endif()
