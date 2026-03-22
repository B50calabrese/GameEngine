/**
 * @file script_components.h
 * @brief ECS components for Lua scripting.
 */

#ifndef INCLUDE_ENGINE_ECS_SCRIPT_COMPONENTS_H_
#define INCLUDE_ENGINE_ECS_SCRIPT_COMPONENTS_H_

#include <string>
#include <vector>

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
};

/**
 * @brief Component that stores custom data for Lua scripts.
 */
struct LuaDataComponent {
  /** @brief Table for storing custom Lua properties. */
  sol::table data;
};

}  // namespace engine::ecs

#endif  // INCLUDE_ENGINE_ECS_SCRIPT_COMPONENTS_H_
