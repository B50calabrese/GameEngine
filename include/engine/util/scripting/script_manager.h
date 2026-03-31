/**
 * @file script_manager.h
 * @brief Manages the Lua scripting environment.
 */

#ifndef INCLUDE_ENGINE_UTIL_SCRIPTING_SCRIPT_MANAGER_H_
#define INCLUDE_ENGINE_UTIL_SCRIPTING_SCRIPT_MANAGER_H_

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace engine::util {

/**
 * @brief Singleton manager for the Lua state and script execution.
 */
class ScriptManager {
 public:
  /** @brief Gets the singleton instance. */
  static ScriptManager& Get() {
    static ScriptManager instance;
    return instance;
  }

  /** @brief Initializes the Lua state. */
  void Init();

  /** @brief Registers a binder callback. */
  void RegisterBinder(std::function<void(sol::state&)> binder);

  /** @brief Runs a Lua file. */
  bool RunFile(const std::string& filepath);

  /** @brief Gets the Lua state. */
  sol::state& GetState() { return lua_; }

  /** @brief Enables hot reloading. */
  void SetHotReloadEnabled(bool enabled) { hot_reload_enabled_ = enabled; }

  /** @brief Sets the asset path. */
  void SetAssetPath(const std::string& path) { asset_path_ = path; }

  /** @brief Checks for script changes. */
  bool CheckForChanges(float dt);

 private:
  ScriptManager() = default;
  ~ScriptManager() = default;

  ScriptManager(const ScriptManager&) = delete;
  ScriptManager& operator=(const ScriptManager&) = delete;

  sol::state lua_;
  bool initialized_ = false;
  std::vector<std::function<void(sol::state&)>> binders_;

  bool hot_reload_enabled_ = false;
  std::string asset_path_;
  float timer_ = 0.0f;
  std::unordered_map<std::string, std::filesystem::file_time_type> file_times_;

  void BindCore();
  void BindMath();
  void BindInput();
  void BindScene();
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_SCRIPTING_SCRIPT_MANAGER_H_
