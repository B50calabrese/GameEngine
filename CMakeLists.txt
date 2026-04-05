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
set(LUA_DIR "${THIRD_PARTY_DIR}/lua")
set(IMGUI_DIR "${THIRD_PARTY_DIR}/imgui")
set(JSON_DIR "${THIRD_PARTY_DIR}/json")

# Inform the linker where to find the library files (e.g., glfw3.lib)
link_directories("${THIRD_PARTY_LIBS}")

# 3. Sources
# ----------

# Lua Sources
file(GLOB LUA_SOURCES "${LUA_DIR}/src/*.c")

# ImGui Sources
set(IMGUI_SOURCES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

# We prefix these with ENGINE_ROOT to ensure they are found correctly
set(SOURCE_FILES
    "${ENGINE_ROOT}/src/engine/core/application.cpp"
    "${ENGINE_ROOT}/src/engine/core/engine.cpp"
    "${ENGINE_ROOT}/src/engine/core/job_system.cpp"
    "${ENGINE_ROOT}/src/engine/core/window.cpp"
    "${ENGINE_ROOT}/src/engine/ecs/systems/physics_system.cpp"
    "${ENGINE_ROOT}/src/engine/ecs/systems/camera_system.cpp"
    "${ENGINE_ROOT}/src/engine/input/input_manager.cpp"
    "${ENGINE_ROOT}/src/engine/input/action_manager.cpp"
    "${ENGINE_ROOT}/src/engine/ui/ui_systems.cpp"
    "${ENGINE_ROOT}/src/engine/scene/scene_manager.cpp"
    "${ENGINE_ROOT}/src/engine/util/logger.cpp"
    "${ENGINE_ROOT}/src/engine/util/easing.cpp"
    "${ENGINE_ROOT}/src/engine/util/tween_manager.cpp"
    "${ENGINE_ROOT}/src/engine/ecs/systems/ai_system.cpp"
    "${ENGINE_ROOT}/src/engine/ecs/entity_manager.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/buffer_utils.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/camera.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/font.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/framebuffer.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/utils/particle_system.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/post_processor.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/lighting_effect.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/ecs/lighting_system.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/primitive_renderer.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/renderer.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/shader.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/utils/sprite_animator.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/ecs/sprite_render_system.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/sprite_sheet.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/text_renderer.cpp"
    "${ENGINE_ROOT}/src/engine/graphics/texture.cpp"
    "${ENGINE_ROOT}/src/engine/util/scripting/script_manager.cpp"
    "${ENGINE_ROOT}/src/engine/ecs/systems/script_system.cpp"
    "${ENGINE_ROOT}/src/engine/ecs/ecs_bindings.cpp"
    "${ENGINE_ROOT}/src/engine/util/console.cpp"
    "${ENGINE_ROOT}/src/tools/leveleditor/component_registry.cpp"
    "${ENGINE_ROOT}/src/tools/leveleditor/level_editor.cpp"
    "${ENGINE_ROOT}/third_party/libs/glad.c"
    ${IMGUI_SOURCES}
)

# Use GLOB_RECURSE relative to CURRENT_SOURCE_DIR
file(GLOB_RECURSE INCLUDE_FILES
    "${ENGINE_ROOT}/include/engine/*.h"
    "${ENGINE_ROOT}/src/engine/*.h"
)

# 4. Create Library
# -----------------
add_library(Lua STATIC ${LUA_SOURCES})
target_include_directories(Lua PRIVATE "${LUA_DIR}/src")

add_library(GameEngine STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

# 5. Linking Dependencies & Includes
# ----------------------------------
target_include_directories(GameEngine 
    PUBLIC  "${ENGINE_ROOT}/include"
    PRIVATE "${ENGINE_ROOT}/src"
    PUBLIC  "${THIRD_PARTY_INCLUDE}"
    PUBLIC  "${THIRD_PARTY_INCLUDE}/freetype"
    PUBLIC  "${LUA_DIR}/include"
    PUBLIC  "${IMGUI_DIR}"
    PUBLIC  "${IMGUI_DIR}/backends"
    PUBLIC  "${JSON_DIR}/include"
)

target_compile_definitions(GameEngine PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_compile_definitions(GameEngine PUBLIC GLFW_INCLUDE_NONE)
target_compile_definitions(GameEngine PRIVATE IMGUI_IMPL_GLFW_DISABLE_WAYLAND)

# Link the GLFW library and all necessary system libraries.
if(WIN32)
    target_link_libraries(GameEngine PRIVATE
        Lua
        "${THIRD_PARTY_LIBS}/glfw3.lib"
        "${THIRD_PARTY_LIBS}/freetype.lib"
        opengl32
        gdi32
        shell32
        user32
    )
else()
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(GLFW glfw3)
        pkg_check_modules(FREETYPE freetype2)
    endif()

    if(GLFW_FOUND AND FREETYPE_FOUND)
        target_link_libraries(GameEngine PUBLIC Lua ${GLFW_LIBRARIES} ${FREETYPE_LIBRARIES} GL dl)
        target_include_directories(GameEngine PUBLIC ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
    else()
        # Fallback for missing pkg-config or headless environments
        message(STATUS "Using fallback linking for GLFW/Freetype")
        target_link_libraries(GameEngine PUBLIC Lua glfw3 freetype GL dl)
    endif()
endif()

# --- 6. Add Demos (Optional when building as a library) ---
# We wrap this in a check so the game doesn't build engine demos by default
if(PROJECT_NAME STREQUAL "GameEngine")
    # Common assets for all demos
    set(COMMON_ASSETS_DIR "${ENGINE_ROOT}/demos/assets")
    set(COMMON_ASSETS_BUILD_DIR "${CMAKE_BINARY_DIR}/demos/assets")

    # Copy common assets once to the build directory
    file(COPY "${COMMON_ASSETS_DIR}/" DESTINATION "${COMMON_ASSETS_BUILD_DIR}")

    # Copy engine shaders to the build directory
    file(COPY "${ENGINE_ROOT}/src/engine/graphics/shaders/" DESTINATION "${CMAKE_BINARY_DIR}/shaders")

    # Helper function to add a demo executable
    function(add_engine_demo DEMO_NAME SOURCE_FILES)
        add_executable(${DEMO_NAME} ${SOURCE_FILES})
        target_link_libraries(${DEMO_NAME} PRIVATE GameEngine DemosCommon)

        # All demos use the shared assets directory
        target_compile_definitions(${DEMO_NAME} PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")

        # Set a consistent output directory for demos
        set_target_properties(${DEMO_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/${DEMO_NAME}"
        )
    endfunction()

    add_subdirectory(demos/common)
    add_subdirectory(demos/ecs-notifications)
    add_subdirectory(demos/scenedemo)
    add_subdirectory(demos/textdemo)
    add_subdirectory(demos/actionmapping)
    add_subdirectory(demos/tweendemo)
    add_subdirectory(demos/spritesheetdemo)
    add_subdirectory(demos/animatordemo)
    add_subdirectory(demos/posteffects)
    add_subdirectory(demos/lightingdemo)
    add_subdirectory(demos/jobsystemdemo)
    add_subdirectory(demos/uimenu)
    add_subdirectory(demos/aidemo)
    add_subdirectory(demos/luademo)
    add_subdirectory(demos/games)
    add_subdirectory(demos/consoledemo)
    add_subdirectory(demos/shapedemo)
    add_subdirectory(demos/componentsdemo)
endif()

# --- 7. Testing ---
option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()

    # Add Google Test
    add_subdirectory("${THIRD_PARTY_DIR}/googletest")

    # Helper to add a test
    function(add_engine_test TEST_NAME SOURCE_FILES)
        add_executable(${TEST_NAME} ${SOURCE_FILES})
        target_link_libraries(${TEST_NAME} PRIVATE GameEngine gtest_main)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

        # Set a consistent output directory for tests
        set_target_properties(${TEST_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
        )
    endfunction()

    # Automatically discover and add tests
    file(GLOB_RECURSE TEST_SOURCES "${ENGINE_ROOT}/src/*_test.cpp")
    foreach(TEST_SOURCE ${TEST_SOURCES})
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        add_engine_test(${TEST_NAME} ${TEST_SOURCE})
    endforeach()
endif()

# --- 8. Tools ---
add_executable(leveleditor "${ENGINE_ROOT}/src/tools/leveleditor/main.cpp")
target_link_libraries(leveleditor PRIVATE GameEngine DemosCommon)
target_compile_definitions(leveleditor PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")
set_target_properties(leveleditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tools"
)
