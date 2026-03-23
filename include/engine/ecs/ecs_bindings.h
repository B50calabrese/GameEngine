/**
 * @file ecs_bindings.h
 * @brief Lua bindings for the ECS framework.
 */

#ifndef INCLUDE_ENGINE_ECS_LUA_BINDINGS_H_
#define INCLUDE_ENGINE_ECS_LUA_BINDINGS_H_

#include <sol/sol.hpp>

#include <engine/ecs/registry.h>

namespace engine::ecs {

/**
 * @brief Utility class for binding ECS systems and components to Lua.
 */
class ECSBindings {
 public:
  /**
   * @brief Binds core ECS types (Registry, EntityID) to Lua.
   * @param lua Reference to the Lua state.
   */
  static void BindCore(sol::state& lua);

  /**
   * @brief Binds standard engine components to Lua.
   * @param lua Reference to the Lua state.
   */
  static void BindComponents(sol::state& lua);

  /**
   * @brief Updates the current registry reference in Lua.
   * @param lua Reference to the Lua state.
   * @param registry Pointer to the active registry.
   */
  static void SetCurrentRegistry(sol::state& lua, Registry* registry);
};

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_LUA_BINDINGS_H_
