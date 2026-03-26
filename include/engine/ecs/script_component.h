/**
 * @file script_component.h
 * @brief ECS component for linking an entity to a Lua script.
 */

#ifndef INCLUDE_ENGINE_ECS_SCRIPT_COMPONENT_H_
#define INCLUDE_ENGINE_ECS_SCRIPT_COMPONENT_H_

#include <string>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace engine::ecs {

/**
 * @brief Component that links an entity to a Lua script.
 */
struct ScriptComponent {
  /** @brief Path to the Lua script. */
  std::string script_path;

  /** @brief Table representing the script instance in Lua. */
  sol::table script_instance = sol::lua_nil;

  /** @brief Whether the on_init hook has been called. */
  bool initialized = false;

  /** @brief Whether to preserve non-function state during hot reload. */
  bool preserve_state = true;
};

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_SCRIPT_COMPONENT_H_
