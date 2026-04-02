#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/lua_data_component.h>
#include <engine/ecs/components/script_component.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/registry.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>
#include <engine/util/scripting/script_manager.h>

#include "../common/demo_utils.h"

class LuaDemoScene : public engine::Scene {
 public:
  LuaDemoScene() : engine::Scene("LuaDemo") {}

  void OnAttach() override {
    LOG_INFO("Entering Lua Demo Scene");

    // Create an entity controlled by Lua
    auto player = GetRegistry().CreateEntity();
    GetRegistry().AddComponent(player, engine::ecs::components::Transform{
                                           {400.0f, 300.0f}, {64.0f, 64.0f}});
    GetRegistry().AddComponent(
        player, engine::ecs::components::Quad{{0.0f, 1.0f, 0.0f, 1.0f}});
    GetRegistry().AddComponent(
        player, engine::ecs::components::Script{ENGINE_ASSETS_PATH
                                                "scripts/player_control.lua"});

    // Create a static entity
    auto wall = GetRegistry().CreateEntity();
    GetRegistry().AddComponent(wall, engine::ecs::components::Transform{
                                         {200.0f, 200.0f}, {100.0f, 100.0f}});
    GetRegistry().AddComponent(
        wall, engine::ecs::components::Quad{{1.0f, 0.0f, 0.0f, 1.0f}});

    LOG_INFO("Lua Demo Scene initialized");
  }

  void OnDetach() override { LOG_INFO("Exiting Lua Demo Scene"); }
};

class LuaDemoApp : public demos::common::BaseDemoApp {
 public:
  void OnDemoInit() override {
    // Register a custom binder to demonstrate extensibility
    engine::util::ScriptManager::Get().RegisterBinder([](sol::state& lua) {
      lua["custom_engine_call"] = []() {
        LOG_INFO("Custom engine call from Lua executed!");
      };
    });

    engine::SceneManager::Get().SetScene(std::make_unique<LuaDemoScene>());
  }
};

int main() {
  engine::EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.window_title = "Lua Scripting Demo";
  config.asset_path = ENGINE_ASSETS_PATH;

  return demos::common::DemoRunner::Run<LuaDemoApp>(config);
}
