cmake_minimum_required(VERSION 3.10)
project(GameEngine VERSION 1.0)

# 1. Configuration
# ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 2. Find Dependencies
# --------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# 3. Executable and Sources
# ------------------------
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_library(GameEngine STATIC ${SOURCE_FILES} "include/engine.h")

# 4. Linking Dependencies
# -----------------------

# Add the include path for GLFW headers
target_include_directories(GameEngine
	PUBLIC "${CMAKE_SOURCE_DIR}/include"
	PRIVATE "${CMAKE_SOURCE_DIR}/src"
	PRIVATE ${GLFW_INCLUDE_DIRS})

# Link the GLFW library
target_link_libraries(GameEngine PRIVATE ${GLFW_LIBRARIES})

# --- 5. Add Demos ---
add_subdirectory(demos/windowdemo)