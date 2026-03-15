add_executable(UiMenuDemo main.cpp)
target_link_libraries(UiMenuDemo PRIVATE GameEngine)
target_compile_definitions(UiMenuDemo PRIVATE ENGINE_ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
