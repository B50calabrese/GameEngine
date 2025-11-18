cmake_minimum_required(VERSION 3.10)
project(GameEngine VERSION 1.0)

# 1. Configuration
# ----------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 2. Define Paths
# ----------------
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(GLFW_INCLUDE_DIR "${THIRD_PARTY_DIR}/include")

# 3. Executable and Sources
# ------------------------
set(SOURCE_FILES
	"src/game_engine/window.cpp"
	"src/game_engine/input_manager.cpp"
	"src/game_engine/engine.cpp"
	"third_party/libs/glad.c"
)
file(GLOB_RECURSE INCLUDE_FILES "src/game_engine/*.h" "include/game_engine/*.h")

add_library(GameEngine STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

# 4. Linking Dependencies
# -----------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(OPENGL REQUIRED opengl)

target_include_directories(GameEngine 
	PUBLIC "${CMAKE_SOURCE_DIR}/include"
	PRIVATE "${CMAKE_SOURCE_DIR}/src"
	PRIVATE ${GLFW_INCLUDE_DIR})

target_link_libraries(GameEngine PRIVATE ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

# --- 5. Add Demos ---
add_subdirectory(demos/windowdemo)
