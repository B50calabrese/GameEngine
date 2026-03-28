#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/physics_components.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/ecs_bindings.h>
#include <engine/ecs/registry.h>

namespace engine::ecs {

void ECSBindings::BindCore(sol::state& lua) {
  lua.new_usertype<Registry>("Registry", "create_entity",
                             &Registry::CreateEntity, "delete_entity",
                             &Registry::DeleteEntity, "is_alive",
                             &Registry::IsAlive, "clear", &Registry::Clear);
}

void ECSBindings::BindComponents(sol::state& lua) {
  // Bind Component Types
  lua.new_usertype<components::Transform>(
      "TransformComponent", "position", &components::Transform::position,
      "scale", &components::Transform::scale, "rotation",
      &components::Transform::rotation);

  lua.new_usertype<components::Sprite>(
      "SpriteComponent", "texture_name", &components::Sprite::texture_name,
      "sprite_sheet_name", &components::Sprite::sprite_sheet_name,
      "sprite_index", &components::Sprite::sprite_index, "tint",
      &components::Sprite::tint, "visible", &components::Sprite::visible);

  lua.new_usertype<components::Velocity>("VelocityComponent", "velocity",
                                         &components::Velocity::velocity);

  // Helper functions for adding/getting components from Lua
  lua["get_transform"] = [&lua](EntityID entity) -> components::Transform& {
    Registry* reg = lua["registry"];
    return reg->GetComponent<components::Transform>(entity);
  };
  lua["has_transform"] = [&lua](EntityID entity) {
    Registry* reg = lua["registry"];
    return reg->HasComponent<components::Transform>(entity);
  };
  lua["add_transform"] = [&lua](EntityID entity, float x, float y) {
    Registry* reg = lua["registry"];
    reg->AddComponent(entity, components::Transform{{x, y}});
  };

  lua["get_sprite"] = [&lua](EntityID entity) -> components::Sprite& {
    Registry* reg = lua["registry"];
    return reg->GetComponent<components::Sprite>(entity);
  };
  lua["has_sprite"] = [&lua](EntityID entity) {
    Registry* reg = lua["registry"];
    return reg->HasComponent<components::Sprite>(entity);
  };
  lua["add_sprite"] = [&lua](EntityID entity, const std::string& texture) {
    Registry* reg = lua["registry"];
    reg->AddComponent(entity, components::Sprite{texture});
  };

  lua["get_velocity"] = [&lua](EntityID entity) -> components::Velocity& {
    Registry* reg = lua["registry"];
    return reg->GetComponent<components::Velocity>(entity);
  };
  lua["has_velocity"] = [&lua](EntityID entity) {
    Registry* reg = lua["registry"];
    return reg->HasComponent<components::Velocity>(entity);
  };
  lua["add_velocity"] = [&lua](EntityID entity, float vx, float vy) {
    Registry* reg = lua["registry"];
    reg->AddComponent(entity, components::Velocity{{vx, vy}});
  };
}

void ECSBindings::SetCurrentRegistry(sol::state& lua, Registry* registry) {
  lua["registry"] = registry;
}

}  // namespace engine::ecs
