add_executable(WindowDemo main.cpp)

# Link against the GameEngine library
target_link_libraries(WindowDemo PRIVATE GameEngine)