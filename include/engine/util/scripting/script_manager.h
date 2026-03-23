/**
 * @file script_manager.h
 * @brief Manages the Lua scripting environment.
 */

#ifndef INCLUDE_ENGINE_UTIL_SCRIPTING_SCRIPT_MANAGER_H_
#define INCLUDE_ENGINE_UTIL_SCRIPTING_SCRIPT_MANAGER_H_

#include <functional>
#include <string>
#include <vector>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace engine::util {

/**
 * @brief Singleton manager for the Lua state and script execution.
 */
class ScriptManager {
 public:
  /**
   * @brief Gets the singleton instance.
   * @returns Reference to the ScriptManager.
   */
  static ScriptManager& Get() {
    static ScriptManager instance;
    return instance;
  }

  /**
   * @brief Initializes the Lua state and binds engine utilities.
   */
  void Init();

  /**
   * @brief Registers a callback to bind custom client logic to Lua.
   * @param binder Function that takes a sol::state& and performs bindings.
   */
  void RegisterBinder(std::function<void(sol::state&)> binder);

  /**
   * @brief Loads and executes a Lua script from a file.
   * @param filepath Path to the Lua script.
   * @returns true if the script was loaded and executed successfully.
   */
  bool RunFile(const std::string& filepath);

  /**
   * @brief Gets the underlying Lua state.
   * @returns Reference to the sol::state.
   */
  sol::state& state() { return lua_; }

 private:
  ScriptManager() = default;
  ~ScriptManager() = default;

  // Delete copy constructor and assignment operator
  ScriptManager(const ScriptManager&) = delete;
  ScriptManager& operator=(const ScriptManager&) = delete;

  sol::state lua_;
  bool initialized_ = false;
  std::vector<std::function<void(sol::state&)>> binders_;

  void BindCore();
  void BindMath();
  void BindInput();
  void BindScene();
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_SCRIPTING_SCRIPT_MANAGER_H_
