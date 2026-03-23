#include <engine/core/transform.h>
#include <engine/ecs/ecs_bindings.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/graphics_components.h>
#include <engine/physics/physics_components.h>

namespace engine::ecs {

void ECSBindings::BindCore(sol::state& lua) {
  lua.new_usertype<Registry>("Registry", "create_entity",
                             &Registry::CreateEntity, "delete_entity",
                             &Registry::DeleteEntity, "is_alive",
                             &Registry::IsAlive, "clear", &Registry::Clear);
}

void ECSBindings::BindComponents(sol::state& lua) {
  // Bind Component Types
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
}

void ECSBindings::SetCurrentRegistry(sol::state& lua, Registry* registry) {
  lua["registry"] = registry;
}

}  // namespace engine::ecs
