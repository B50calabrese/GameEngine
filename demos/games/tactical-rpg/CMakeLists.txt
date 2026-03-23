set(TACTICAL_RPG_SOURCES
    main.cpp
    battle_scene.cpp
    game_manager.cpp
)

add_executable(TacticalRpg ${TACTICAL_RPG_SOURCES})
target_link_libraries(TacticalRpg PRIVATE GameEngine DemosCommon)

target_compile_definitions(TacticalRpg PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")

set_target_properties(TacticalRpg PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/TacticalRpg"
)
