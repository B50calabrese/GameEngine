/**
 * @file physics_system.cpp
 * @brief Implementation of the physics system.
 */

#include <algorithm>
#include <vector>

#include <engine/ecs/components/physics_components.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/systems/physics_system.h>
#include <engine/util/collision.h>

namespace engine::ecs::systems {

void PhysicsSystem::Update(Registry* registry, float dt) {
  if (!registry) {
    return;
  }
  // 1. Apply Gravity
  auto gravity_view = registry->GetView<engine::ecs::components::Transform,
                                        engine::ecs::components::Velocity,
                                        engine::ecs::components::Gravity>();
  for (auto entity : gravity_view) {
    auto& velocity =
        registry->GetComponent<engine::ecs::components::Velocity>(entity);
    auto& gravity =
        registry->GetComponent<engine::ecs::components::Gravity>(entity);
    velocity.velocity.y -= gravity.strength * dt;
  }

  // 2. Perform Movement and Collision Resolution (Separated passes)
  auto collider_view = registry->GetView<engine::ecs::components::Transform,
                                         engine::ecs::components::Collider>();
  std::vector<EntityID> colliders(collider_view.begin(), collider_view.end());

  // 2a. Update Positions (Horizontal)
  auto velocity_view = registry->GetView<engine::ecs::components::Transform,
                                         engine::ecs::components::Velocity>();
  for (auto entity : velocity_view) {
    auto& transform =
        registry->GetComponent<engine::ecs::components::Transform>(entity);
    auto& velocity =
        registry->GetComponent<engine::ecs::components::Velocity>(entity);
    transform.position.x += velocity.velocity.x * dt;
  }

  // 2b. Resolve Horizontal Collisions
  for (size_t i = 0; i < colliders.size(); ++i) {
    for (size_t j = i + 1; j < colliders.size(); ++j) {
      EntityID a = colliders[i];
      EntityID b = colliders[j];

      auto& trans_a =
          registry->GetComponent<engine::ecs::components::Transform>(a);
      auto& col_a =
          registry->GetComponent<engine::ecs::components::Collider>(a);
      auto& trans_b =
          registry->GetComponent<engine::ecs::components::Transform>(b);
      auto& col_b =
          registry->GetComponent<engine::ecs::components::Collider>(b);

      if (util::CheckAABB(trans_a.position + col_a.offset, col_a.size,
                          trans_b.position + col_b.offset, col_b.size)) {
        // Notify both parties of the collision (if callbacks are present)
        if (col_a.on_collision) {
          col_a.on_collision(a, b);
        }
        if (col_b.on_collision) {
          col_b.on_collision(b, a);
        }

        // Resolve unless one is a trigger
        if (!col_a.is_trigger && !col_b.is_trigger) {
          if (!col_a.is_static &&
              registry->HasComponent<engine::ecs::components::Velocity>(a)) {
            auto& vel_a =
                registry->GetComponent<engine::ecs::components::Velocity>(a);
            if (vel_a.velocity.x > 0) {
              trans_a.position.x = trans_b.position.x + col_b.offset.x -
                                   col_a.size.x - col_a.offset.x;
            } else if (vel_a.velocity.x < 0) {
              trans_a.position.x = trans_b.position.x + col_b.offset.x +
                                   col_b.size.x - col_a.offset.x;
            }
          }
        }
      }
    }
  }

  // 2c. Update Positions (Vertical)
  for (auto entity : velocity_view) {
    auto& transform =
        registry->GetComponent<engine::ecs::components::Transform>(entity);
    auto& velocity =
        registry->GetComponent<engine::ecs::components::Velocity>(entity);
    transform.position.y += velocity.velocity.y * dt;
  }

  // 2d. Resolve Vertical Collisions
  for (size_t i = 0; i < colliders.size(); ++i) {
    for (size_t j = i + 1; j < colliders.size(); ++j) {
      EntityID a = colliders[i];
      EntityID b = colliders[j];

      auto& trans_a =
          registry->GetComponent<engine::ecs::components::Transform>(a);
      auto& col_a =
          registry->GetComponent<engine::ecs::components::Collider>(a);
      auto& trans_b =
          registry->GetComponent<engine::ecs::components::Transform>(b);
      auto& col_b =
          registry->GetComponent<engine::ecs::components::Collider>(b);

      if (util::CheckAABB(trans_a.position + col_a.offset, col_a.size,
                          trans_b.position + col_b.offset, col_b.size)) {
        // Horizontal already called the on_collision callback if there was an
        // overlap initially. However, if movement in Y creates a *new* overlap,
        // we might need to notify here too. Let's call them anyway but it could
        // be redundant for some frames.
        if (col_a.on_collision) {
          col_a.on_collision(a, b);
        }
        if (col_b.on_collision) {
          col_b.on_collision(b, a);
        }

        if (!col_a.is_trigger && !col_b.is_trigger) {
          if (!col_a.is_static &&
              registry->HasComponent<engine::ecs::components::Velocity>(a)) {
            auto& vel_a =
                registry->GetComponent<engine::ecs::components::Velocity>(a);
            if (vel_a.velocity.y < 0) {
              trans_a.position.y = trans_b.position.y + col_b.offset.y +
                                   col_b.size.y - col_a.offset.y;
              vel_a.velocity.y = 0;
            } else if (vel_a.velocity.y > 0) {
              trans_a.position.y = trans_b.position.y + col_b.offset.y -
                                   col_a.size.y - col_a.offset.y;
              vel_a.velocity.y = 0;
            }
          }
        }
      }
    }
  }
}

}  // namespace engine::ecs::systems
