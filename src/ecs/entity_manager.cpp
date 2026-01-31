#include "ecs/entity_manager.h"

#include <algorithm>

namespace engine::ecs {

EntityID EntityManager::CreateEntity() {
  // Pop a free entity.
  if (free_entities.size() > 0) {
    int ret = free_entities[0];
    free_entities.erase(free_entities.begin());
    return ret;
  }
  return next_id++;
}

void EntityManager::DestroyEntity(EntityID entity) {
  free_entities.push_back(entity);
}

bool EntityManager::IsAlive(EntityID entity) const {
  if (entity < next_id) {
    return true;
  }
  auto it = std::find(free_entities.begin(), free_entities.end(), entity);
  // If it is at the end of the vector then it is alive.
  return it == free_entities.end();
}
}  // namespace engine::ecs