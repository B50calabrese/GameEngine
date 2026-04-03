/**
 * @file lighting_system.cpp
 * @brief Syncs ECS light components with the LightingEffect.
 */

#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/ecs/lighting_system.h>
#include <engine/graphics/lighting_effect.h>

namespace engine::graphics::ecs {

/**
 * @brief Updates the LightingEffect with data from ECS components.
 * @param registry The ECS registry.
 * @param lighting_effect The lighting effect to update.
 */
void UpdateLightingSystem(engine::ecs::Registry* registry,
                          LightingEffect* lighting_effect) {
  if (!registry || !lighting_effect) {
    return;
  }

  lighting_effect->ClearLights();
  lighting_effect->ClearOccluders();

  // Sync Lights
  auto light_view = registry->GetView<engine::ecs::components::Transform,
                                      engine::ecs::components::Light>();
  for (auto entity : light_view) {
    const auto& transform =
        registry->GetComponent<engine::ecs::components::Transform>(entity);
    const auto& light_comp =
        registry->GetComponent<engine::ecs::components::Light>(entity);

    engine::graphics::Light light;
    light.position = transform.position;
    light.color = light_comp.color;
    light.intensity = light_comp.intensity;
    light.radius = light_comp.radius;
    light.angle = light_comp.angle;
    light.direction = light_comp.direction;
    light.is_directional = light_comp.is_directional;
    light.dir_vector = light_comp.dir_vector;

    lighting_effect->AddLight(light);
  }

  // Sync Occluders
  auto occluder_view = registry->GetView<engine::ecs::components::Transform,
                                         engine::ecs::components::Occluder>();
  for (auto entity : occluder_view) {
    const auto& transform =
        registry->GetComponent<engine::ecs::components::Transform>(entity);
    const auto& occluder_comp =
        registry->GetComponent<engine::ecs::components::Occluder>(entity);

    engine::graphics::Occluder occluder;
    occluder.position = transform.position;
    occluder.size = occluder_comp.size * transform.scale;
    occluder.rotation = transform.rotation;

    lighting_effect->AddOccluder(occluder);
  }
}

}  // namespace engine::graphics::ecs
