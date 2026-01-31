#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstdint>
#include <vector>

namespace engine::ecs {

using EntityID = uint32_t;

class EntityManager {
 public:
  EntityID CreateEntity();

  void DestroyEntity(EntityID entity);

  bool IsAlive(EntityID entity) const;

 private:
  EntityID next_id = 0;
  std::vector<EntityID> free_entities;
};
}  // namespace engine::ecs

#endif  // ENTITY_MANAGER_H