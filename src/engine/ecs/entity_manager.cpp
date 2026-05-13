/**
 * @file entity_manager.cpp
 * @brief EntityManager class implementation.
 */

/**
 * @dir src/engine/ecs
 * @brief ECS internal logic.
 */

#include <algorithm>

#include <engine/ecs/entity_manager.h>

namespace engine::ecs {

EntityID EntityManager::CreateEntity() {
  // Pop a free entity.
  if (free_entities_.size() > 0) {
    int ret = free_entities_[0];
    free_entities_.erase(free_entities_.begin());
    return ret;
  }
  return next_id_++;
}

void EntityManager::DestroyEntity(EntityID entity) {
  free_entities_.push_back(entity);
}

bool EntityManager::IsAlive(EntityID entity) const {
  auto it = std::find(free_entities_.begin(), free_entities_.end(), entity);
  return entity < next_id_ && it == free_entities_.end();
}

size_t EntityManager::GetEntityCount() const {
  return static_cast<size_t>(next_id_) - free_entities_.size();
}

void EntityManager::Clear() {
  next_id_ = 0;
  free_entities_.clear();
}
}  // namespace engine::ecs
