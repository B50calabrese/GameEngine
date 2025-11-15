cmake_minimum_required(VERSION 3.10)
project(GameEngine VERSION 1.0)

# 1. Configuration
# ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 2. Define Paths
# ----------------
# Use CMAKE_SOURCE_DIR for relative pathing from the root CMakeLists.txt
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(GLFW_INCLUDE_DIR "${THIRD_PARTY_DIR}/include")
set(GLFW_LIBRARY_DIR "${THIRD_PARTY_DIR}/libs")

# Inform the linker where to find the library files (e.g., glfw3.lib)
link_directories(${GLFW_LIBRARY_DIR})

# 3. Executable and Sources
# ------------------------
set(SOURCE_FILES
	"src/window.cpp"
	"src/input_manager.cpp"
	"src/engine.cpp"
	"third_party/libs/glad.c"
)
file(GLOB_RECURSE INCLUDE_FILES "src/*.h" "include/*.h")

add_library(GameEngine STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

# 4. Linking Dependencies
# -----------------------

# Add the include path for GLFW headers (e.g., allows #include <GLFW/glfw3.h>)
# We use PRIVATE because only the MyApp source files need these headers.
target_include_directories(GameEngine 
	PUBLIC "${CMAKE_SOURCE_DIR}/include"
	PRIVATE "${CMAKE_SOURCE_DIR}/src"
	PRIVATE ${GLFW_INCLUDE_DIR})

# Link the GLFW library and all necessary system libraries.
target_link_libraries(GameEngine PRIVATE glfw3 opengl32 gdi32 shell32 user32)

# --- 5. Add Demos ---
# Instruct CMake to process the CMakeLists.txt files in the specified subdirectories.
add_subdirectory(demos/windowdemo)