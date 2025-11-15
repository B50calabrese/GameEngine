find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

file(GLOB SOURCE_FILES "*.cpp")

add_library(GameEngine STATIC ${SOURCE_FILES} "${CMAKE_SOURCE_DIR}/include/engine.h")

target_include_directories(GameEngine
  PUBLIC "${CMAKE_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_SOURCE_DIR}/src"
  PRIVATE ${GLFW_INCLUDE_DIRS})

target_link_libraries(GameEngine PRIVATE ${GLFW_LIBRARIES})