add_executable(AnimatorDemo main.cpp)
target_link_libraries(AnimatorDemo PRIVATE GameEngine)
target_compile_definitions(AnimatorDemo PRIVATE ENGINE_ASSETS_PATH="${ENGINE_ROOT}/demos/spritesheetdemo/assets")
