set(TACTICAL_RPG_SOURCES
    main.cpp
    battle_scene.cpp
    game_manager.cpp
    battle_grid.cpp
    turn_manager.cpp
    enemy_ai.cpp
    class_registry.cpp
    grid_renderer.cpp
    map_node.cpp
    systems/sync_transform_system.cpp
    systems/combat_system.cpp
    character_renderer.cpp
    action_registry.cpp
    classes/barbarian.cpp
    classes/bard.cpp
    classes/cleric.cpp
    classes/druid.cpp
    classes/fighter.cpp
    classes/monk.cpp
    classes/paladin.cpp
    classes/ranger.cpp
    classes/rogue.cpp
    classes/sorcerer.cpp
    classes/warlock.cpp
    classes/wizard.cpp
)

add_executable(TacticalRpg ${TACTICAL_RPG_SOURCES})
target_link_libraries(TacticalRpg PRIVATE GameEngine DemosCommon)

target_compile_definitions(TacticalRpg PRIVATE ENGINE_ASSETS_PATH="${COMMON_ASSETS_BUILD_DIR}/")

set_target_properties(TacticalRpg PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/demos/TacticalRpg"
)
