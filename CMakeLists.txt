cmake_minimum_required(VERSION 3.10)
project(GameEngine VERSION 1.0)

# 1. Configuration
# ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 2. Define Paths
# ----------------
# Use CMAKE_SOURCE_DIR for relative pathing from the root CMakeLists.txt
set(ENGINE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(THIRD_PARTY_DIR "${ENGINE_ROOT}/third_party")
set(GLFW_INCLUDE_DIR "${THIRD_PARTY_DIR}/include")
set(THIRD_PARTY_LIBS "${THIRD_PARTY_DIR}/libs")
set(GLM_INCLUDE_DIR  "${THIRD_PARTY_DIR}/include")

# Inform the linker where to find the library files (e.g., glfw3.lib)
link_directories(${THIRD_PARTY_LIBS})

# 3. Executable and Sources
# ------------------------
set(SOURCE_FILES
    "${ENGINE_ROOT}/src/application.cpp"
	"${ENGINE_ROOT}/src/engine.cpp"
	"${ENGINE_ROOT}/src/input_manager.cpp"
	"${ENGINE_ROOT}/src/scene_manager.cpp"
    "${ENGINE_ROOT}/src/window.cpp"
	"${ENGINE_ROOT}/src/graphics/buffer_utils.cpp"
	"${ENGINE_ROOT}/src/graphics/camera.cpp"
	"${ENGINE_ROOT}/src/graphics/primitive_renderer.cpp"
    "${ENGINE_ROOT}/src/graphics/renderer.cpp"
	"${ENGINE_ROOT}/src/graphics/shader.cpp"
	"${ENGINE_ROOT}/src/graphics/texture.cpp"
	"${THIRD_PARTY_LIBS}/glad.c"
)
file(GLOB_RECURSE INCLUDE_FILES "${ENGINE_ROOT}/src/*.h" "${ENGINE_ROOT}/include/*.h" "${ENGINE_ROOT}/include/graphics/*.h")

add_library(GameEngine STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

# 4. Linking Dependencies
# -----------------------

# Add the include path for GLFW headers (e.g., allows #include <GLFW/glfw3.h>)
# We use PRIVATE because only the MyApp source files need these headers.
target_include_directories(GameEngine 
	PRIVATE "${GLFW_INCLUDE_DIR}"	
	PUBLIC "${CMAKE_SOURCE_DIR}/include"
	PRIVATE "${CMAKE_SOURCE_DIR}/src"
	PUBLIC  "${GLM_INCLUDE_DIR}")

# Link the GLFW library and all necessary system libraries.
target_link_libraries(GameEngine PRIVATE glfw3 opengl32 gdi32 shell32 user32)

# --- 5. Add Demos ---
# Instruct CMake to process the CMakeLists.txt files in the specified subdirectories.
add_subdirectory(demos/applicationdemo)
add_subdirectory(demos/windowdemo)
add_subdirectory(demos/rendererdemo)
add_subdirectory(demos/scenedemo)