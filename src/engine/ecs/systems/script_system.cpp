#include <engine/ecs/components/lua_data_component.h>
#include <engine/ecs/components/script_component.h>
#include <engine/ecs/ecs_bindings.h>
#include <engine/ecs/systems/script_system.h>
#include <engine/util/logger.h>
#include <engine/util/scripting/script_manager.h>

namespace engine::ecs::systems {

bool ScriptSystem::initialized_ = false;

void ScriptSystem::Init(Registry* registry) {
  if (initialized_) {
    return;
  }

  auto& lua = util::ScriptManager::Get().state();

  ECSBindings::BindCore(lua);
  ECSBindings::BindComponents(lua);
  ECSBindings::SetCurrentRegistry(lua, registry);

  initialized_ = true;
}

void ScriptSystem::Update(Registry* registry, float dt) {
  if (!registry) return;

  auto& lua = util::ScriptManager::Get().state();
  ECSBindings::SetCurrentRegistry(lua, registry);

  bool reload_all = util::ScriptManager::Get().CheckForChanges(dt);

  auto view = registry->GetView<engine::ecs::components::Script>();

  for (auto entity : view) {
    auto& comp =
        registry->GetComponent<engine::ecs::components::Script>(entity);
    if (comp.script_path.empty()) continue;

    // Load script if not already loaded OR if hot reload triggered
    if (!comp.script_instance.valid() || reload_all) {
      sol::table old_instance = comp.script_instance;
      auto result = lua.script_file(comp.script_path);
      if (result.valid()) {
        sol::object obj = result;
        if (obj.get_type() == sol::type::table) {
          comp.script_instance = obj.as<sol::table>();

          // Preserve state if requested
          if (comp.preserve_state && old_instance.valid()) {
            for (auto const& [key, value] : old_instance) {
              // Only copy non-function values to preserve state while picking
              // up new logic
              if (value.get_type() != sol::type::function) {
                comp.script_instance[key] = value;
              }
            }
          }
          if (reload_all) {
            comp.initialized = false;
          }
        } else {
          LOG_ERR("Lua script must return a table: %s",
                  comp.script_path.c_str());
          continue;
        }
      } else {
        sol::error err = result;
        LOG_ERR("Failed to load script: %s\n%s", comp.script_path.c_str(),
                err.what());
        continue;
      }
    }

    // Call on_init if needed
    if (!comp.initialized) {
      comp.script_instance["entity"] = entity;
      sol::safe_function on_init = comp.script_instance["on_init"];
      if (on_init.valid()) {
        auto result = on_init(comp.script_instance);
        if (!result.valid()) {
          sol::error err = result;
          LOG_ERR("Error in on_init (%s): %s", comp.script_path.c_str(),
                  err.what());
        }
      }
      comp.initialized = true;
    }

    // Call on_update
    sol::safe_function on_update = comp.script_instance["on_update"];
    if (on_update.valid()) {
      auto result = on_update(comp.script_instance, dt);
      if (!result.valid()) {
        sol::error err = result;
        LOG_ERR("Error in on_update (%s): %s", comp.script_path.c_str(),
                err.what());
      }
    }
  }
}

}  // namespace engine::ecs::systems
