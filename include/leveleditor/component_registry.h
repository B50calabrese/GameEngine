#ifndef INCLUDE_LEVELEDITOR_COMPONENT_REGISTRY_H_
#define INCLUDE_LEVELEDITOR_COMPONENT_REGISTRY_H_

#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include <engine/ecs/registry.h>

namespace leveleditor {

/**
 * @brief Registry for ECS components to enable UI editing and serialization.
 */
class ComponentRegistry {
 public:
  struct ComponentInfo {
    std::string name;
    std::function<void(engine::ecs::EntityID, engine::ecs::Registry&)> ui_func;
    std::function<void(engine::ecs::EntityID, engine::ecs::Registry&,
                       nlohmann::json&)>
        serialize_func;
    std::function<void(engine::ecs::EntityID, engine::ecs::Registry&,
                       const nlohmann::json&)>
        deserialize_func;
    std::function<void(engine::ecs::EntityID, engine::ecs::Registry&)> add_func;
    std::function<void(engine::ecs::EntityID, engine::ecs::Registry&)>
        remove_func;
    std::function<bool(engine::ecs::EntityID, engine::ecs::Registry&)> has_func;
  };

  static ComponentRegistry& Get();

  void RegisterComponent(const ComponentInfo& info);
  const std::map<std::string, ComponentInfo>& GetComponents() const {
    return components_;
  }

  /**
   * @brief Populates the registry with base engine components.
   */
  void Init();

 private:
  std::map<std::string, ComponentInfo> components_;
};

}  // namespace leveleditor

#endif  // INCLUDE_LEVELEDITOR_COMPONENT_REGISTRY_H_
