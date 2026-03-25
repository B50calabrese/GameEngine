#include <engine/core/application.h>
#include <engine/core/transform.h>
#include <engine/ecs/lua_data_component.h>
#include <engine/ecs/registry.h>
#include <engine/ecs/script_component.h>
#include <engine/graphics/graphics_components.h>
#include <engine/scene/scene.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>
#include <engine/util/scripting/script_manager.h>

using namespace engine;

class LuaDemoScene : public Scene {
 public:
  LuaDemoScene() : Scene("LuaDemo") {}

  void OnAttach() override {
    LOG_INFO("Entering Lua Demo Scene");

    // Create an entity controlled by Lua
    auto player = registry().CreateEntity();
    registry().AddComponent(
        player, core::TransformComponent{{400.0f, 300.0f}, {64.0f, 64.0f}});
    registry().AddComponent(player, graphics::SpriteComponent{"player.png"});
    registry().AddComponent(
        player,
        ecs::ScriptComponent{ENGINE_ASSETS_PATH "scripts/player_control.lua"});

    // Create a static entity
    auto wall = registry().CreateEntity();
    registry().AddComponent(
        wall, core::TransformComponent{{200.0f, 200.0f}, {100.0f, 100.0f}});
    registry().AddComponent(wall, graphics::SpriteComponent{"wall.png"});

    LOG_INFO("Lua Demo Scene initialized");
  }

  void OnUpdate(float dt) override {
    // Scene-specific update logic (optional)
  }

  void OnDetach() override { LOG_INFO("Exiting Lua Demo Scene"); }
};

class LuaDemoApp : public Application {
 public:
  void OnInit() override {
    // Register a custom binder to demonstrate extensibility
    util::ScriptManager::Get().RegisterBinder([](sol::state& lua) {
      lua["custom_engine_call"] = []() {
        LOG_INFO("Custom engine call from Lua executed!");
      };
    });

    SceneManager::Get().SetScene(std::make_unique<LuaDemoScene>());
  }

  void OnUpdate(double dt) override {}
  void OnShutdown() override {}
};

int main() {
  EngineConfig config;
  config.window_width = 800;
  config.window_height = 600;
  config.window_title = "Lua Scripting Demo";
  config.asset_path = ENGINE_ASSETS_PATH;

  Engine::Init(config);
  LuaDemoApp app;
  app.Run();

  return 0;
}
