#include <engine/core/application.h>
#include <engine/ecs/components/camera_component.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/systems/camera_system.h>
#include <engine/graphics/camera.h>

namespace engine::ecs::systems {

void CameraSystem::Update(Registry* registry) {
  if (!registry) {
    return;
  }

  auto view = registry->GetView<engine::ecs::components::CameraComponent,
                                engine::ecs::components::Transform>();
  for (auto entity : view) {
    auto& camera_comp =
        registry->GetComponent<engine::ecs::components::CameraComponent>(
            entity);
    if (camera_comp.is_active) {
      auto& transform =
          registry->GetComponent<engine::ecs::components::Transform>(entity);

      auto& main_camera = Application::Get().camera();
      main_camera.set_position({transform.position.x + camera_comp.offset_x,
                                transform.position.y + camera_comp.offset_y,
                                0.0f});

      // Note: Camera class doesn't currently support zoom in its public API
      // in a simple way without recalculating projection.
      // For now we just sync position.

      break;  // Only use the first active camera
    }
  }
}

}  // namespace engine::ecs::systems
