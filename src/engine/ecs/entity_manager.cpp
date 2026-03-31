/**
 * @file entity_manager.cpp
 * @brief EntityManager class implementation.
 */

#include <algorithm>

#include <engine/ecs/entity_manager.h>

namespace engine::ecs {

EntityID EntityManager::CreateEntity() {
  if (!free_entities_.empty()) {
    EntityID entity = free_entities_.back();
    free_entities_.pop_back();
    return entity;
  }
  return next_id_++;
}

void EntityManager::DestroyEntity(EntityID entity) {
  free_entities_.push_back(entity);
}

bool EntityManager::IsAlive(EntityID entity) const {
  if (entity >= next_id_) return false;
  auto it = std::find(free_entities_.begin(), free_entities_.end(), entity);
  return it == free_entities_.end();
}

void EntityManager::Clear() {
  next_id_ = 0;
  free_entities_.clear();
}

}  // namespace engine::ecs
