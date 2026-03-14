cmake_minimum_required(VERSION 3.10)
project(JobSystemDemo)

set(CMAKE_CXX_STANDARD 17)

add_executable(JobSystemDemo main.cpp)

target_link_libraries(JobSystemDemo PRIVATE GameEngine)

# Copy assets to the build directory
add_custom_command(TARGET JobSystemDemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:JobSystemDemo>/assets
)
