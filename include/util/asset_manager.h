/**
 * @file asset_manager.h
 * @brief Template-based asset caching and management.
 */

#ifndef INCLUDE_UTIL_ASSET_MANAGER_H_
#define INCLUDE_UTIL_ASSET_MANAGER_H_

#include <memory>
#include <string>
#include <unordered_map>

/**
 * @namespace engine::util
 * @brief General purpose utility classes.
 */
namespace engine::util {

/**
 * @brief Centralized, templated utility for managing the lifecycle of engine
 * resources.
 *
 * Uses reference counting (std::shared_ptr/std::weak_ptr) to automatically free
 * assets when they are no longer in use.
 */
template <typename T>
class AssetManager {
 public:
  /**
   * @brief Retrieves a reference-counted asset.
   *
   * If the asset is already in the cache and still alive, it returns it.
   * Otherwise, it calls T::Load(path) to create it.
   *
   * @param path The path to the asset.
   * @return A shared pointer to the asset, or nullptr if loading fails.
   */
  static std::shared_ptr<T> Get(const std::string& path) {
    auto& cache = GetCache();

    if (cache.count(path) && !cache[path].expired()) {
      return cache[path].lock();
    }

    // T must implement a static Load method returning std::shared_ptr<T>
    std::shared_ptr<T> asset = T::Load(path);
    if (asset) {
      cache[path] = asset;
    }
    return asset;
  }

  /**
   * @brief Clears references to assets that are no longer in use.
   */
  static void GarbageCollect() {
    auto& cache = GetCache();
    for (auto it = cache.begin(); it != cache.end();) {
      if (it->second.expired()) {
        it = cache.erase(it);
      } else {
        ++it;
      }
    }
  }

 private:
  /**
   * @brief Static cache of weak pointers to assets.
   * @return Reference to the cache.
   */
  static std::unordered_map<std::string, std::weak_ptr<T>>& GetCache() {
    static std::unordered_map<std::string, std::weak_ptr<T>> cache;
    return cache;
  }
};

}  // namespace engine::util

#endif  // INCLUDE_UTIL_ASSET_MANAGER_H_
