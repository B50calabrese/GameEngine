/**
 * @file lighting_system.cpp
 * @brief Syncs ECS light components with the LightingEffect.
 */

#include <engine/core/transform.h>
#include <engine/ecs/registry.h>
#include <engine/graphics/graphics_components.h>
#include <engine/graphics/lighting_effect.h>

namespace engine::graphics {

/**
 * @brief Updates the LightingEffect with data from ECS components.
 * @param registry The ECS registry.
 * @param lighting_effect The lighting effect to update.
 */
void UpdateLightingSystem(ecs::Registry* registry,
                          LightingEffect* lighting_effect) {
  if (!registry || !lighting_effect) {
    return;
  }

  lighting_effect->ClearLights();
  lighting_effect->ClearOccluders();

  // Sync Lights
  auto light_view =
      registry->GetView<core::TransformComponent, LightComponent>();
  for (auto entity : light_view) {
    const auto& transform =
        registry->GetComponent<core::TransformComponent>(entity);
    const auto& light_comp = registry->GetComponent<LightComponent>(entity);

    Light light;
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
  auto occluder_view =
      registry->GetView<core::TransformComponent, OccluderComponent>();
  for (auto entity : occluder_view) {
    const auto& transform =
        registry->GetComponent<core::TransformComponent>(entity);
    const auto& occluder_comp =
        registry->GetComponent<OccluderComponent>(entity);

    Occluder occluder;
    occluder.position = transform.position;
    occluder.size = occluder_comp.size * transform.scale;
    occluder.rotation = transform.rotation;

    lighting_effect->AddOccluder(occluder);
  }
}

}  // namespace engine::graphics
