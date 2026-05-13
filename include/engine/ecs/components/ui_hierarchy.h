/**
 * @file ui_hierarchy.h
 * @brief Relationship component for UI nesting.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_UI_HIERARCHY_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_UI_HIERARCHY_H_

#include <vector>

#include <engine/ecs/entity_manager.h>

namespace engine::ecs::components {

/**
 * @brief Relationship component for UI nesting.
 */
struct UiHierarchy {
  EntityID parent = kInvalidEntity;
  std::vector<EntityID> children;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_UI_HIERARCHY_H_
