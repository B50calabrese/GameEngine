/**
 * @file lua_data_component.h
 * @brief ECS component for storing custom Lua data.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_LUA_DATA_COMPONENT_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_LUA_DATA_COMPONENT_H_

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace engine::ecs::components {

/**
 * @brief Component that stores custom data for Lua scripts.
 */
struct LuaData {
  /** @brief Table for storing custom Lua properties. */
  sol::table data;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_LUA_DATA_COMPONENT_H_
