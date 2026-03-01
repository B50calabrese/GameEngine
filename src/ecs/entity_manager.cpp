#include "ecs/entity_manager.h"

#include <algorithm>

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
  if (entity < next_id_) {
    return true;
  }
  auto it = std::find(free_entities_.begin(), free_entities_.end(), entity);
  // If it is at the end of the vector then it is alive.
  return it != free_entities_.end();
}
}  // namespace engine::ecs
