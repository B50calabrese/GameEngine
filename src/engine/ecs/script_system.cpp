#include <engine/core/transform.h>
#include <engine/ecs/script_components.h>
#include <engine/ecs/script_system.h>
#include <engine/graphics/graphics_components.h>
#include <engine/physics/physics_components.h>
#include <engine/util/logger.h>
#include <engine/util/scripting/script_manager.h>

namespace engine::ecs {

bool ScriptSystem::initialized_ = false;

void ScriptSystem::Init(Registry* registry) {
  if (initialized_) {
    return;
  }

  auto& lua = util::ScriptManager::Get().state();

  // Bind Registry
  lua.new_usertype<Registry>("Registry", "create_entity",
                             &Registry::CreateEntity, "delete_entity",
                             &Registry::DeleteEntity, "is_alive",
                             &Registry::IsAlive, "clear", &Registry::Clear);

  lua["registry"] = registry;

  // Bind Components
  lua.new_usertype<core::TransformComponent>(
      "TransformComponent", "position", &core::TransformComponent::position,
      "scale", &core::TransformComponent::scale, "rotation",
      &core::TransformComponent::rotation);

  lua.new_usertype<graphics::SpriteComponent>(
      "SpriteComponent", "texture_name",
      &graphics::SpriteComponent::texture_name, "sprite_sheet_name",
      &graphics::SpriteComponent::sprite_sheet_name, "sprite_index",
      &graphics::SpriteComponent::sprite_index, "tint",
      &graphics::SpriteComponent::tint, "visible",
      &graphics::SpriteComponent::visible);

  lua.new_usertype<physics::VelocityComponent>(
      "VelocityComponent", "velocity", &physics::VelocityComponent::velocity);

  // Helper functions for adding/getting components from Lua
  lua["get_transform"] = [&lua](EntityID entity) -> core::TransformComponent& {
    Registry* reg = lua["registry"];
    return reg->GetComponent<core::TransformComponent>(entity);
  };
  lua["has_transform"] = [&lua](EntityID entity) {
    Registry* reg = lua["registry"];
    return reg->HasComponent<core::TransformComponent>(entity);
  };
  lua["add_transform"] = [&lua](EntityID entity, float x, float y) {
    Registry* reg = lua["registry"];
    reg->AddComponent(entity, core::TransformComponent{{x, y}});
  };

  lua["get_sprite"] = [&lua](EntityID entity) -> graphics::SpriteComponent& {
    Registry* reg = lua["registry"];
    return reg->GetComponent<graphics::SpriteComponent>(entity);
  };
  lua["has_sprite"] = [&lua](EntityID entity) {
    Registry* reg = lua["registry"];
    return reg->HasComponent<graphics::SpriteComponent>(entity);
  };
  lua["add_sprite"] = [&lua](EntityID entity, const std::string& texture) {
    Registry* reg = lua["registry"];
    reg->AddComponent(entity, graphics::SpriteComponent{texture});
  };

  lua["get_velocity"] = [&lua](EntityID entity) -> physics::VelocityComponent& {
    Registry* reg = lua["registry"];
    return reg->GetComponent<physics::VelocityComponent>(entity);
  };
  lua["has_velocity"] = [&lua](EntityID entity) {
    Registry* reg = lua["registry"];
    return reg->HasComponent<physics::VelocityComponent>(entity);
  };
  lua["add_velocity"] = [&lua](EntityID entity, float vx, float vy) {
    Registry* reg = lua["registry"];
    reg->AddComponent(entity, physics::VelocityComponent{{vx, vy}});
  };

  initialized_ = true;
}

void ScriptSystem::Update(Registry* registry, float dt) {
  if (!registry) return;

  auto& lua = util::ScriptManager::Get().state();
  lua["registry"] = registry;

  auto view = registry->GetView<ScriptComponent>();

  for (auto entity : view) {
    auto& comp = registry->GetComponent<ScriptComponent>(entity);
    if (comp.script_path.empty()) continue;

    // Load script if not already loaded
    if (!comp.script_instance.valid()) {
      auto result = lua.script_file(comp.script_path);
      if (result.valid()) {
        if (result.get_type() == sol::type::table) {
          comp.script_instance = result;
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

}  // namespace engine::ecs
