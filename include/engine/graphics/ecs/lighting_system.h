/**
 * @file lighting_system.h
 * @brief Syncs ECS light components with the LightingEffect.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_ECS_LIGHTING_SYSTEM_H_
#define INCLUDE_ENGINE_GRAPHICS_ECS_LIGHTING_SYSTEM_H_

namespace engine::ecs {
class Registry;
}

namespace engine::graphics {
class LightingEffect;
}

namespace engine::graphics::ecs {

/**
 * @brief Updates the LightingEffect with data from ECS components.
 * @param registry The ECS registry.
 * @param lighting_effect The lighting effect to update.
 */
void UpdateLightingSystem(engine::ecs::Registry* registry,
                          engine::graphics::LightingEffect* lighting_effect);

}  // namespace engine::graphics::ecs

#endif  // INCLUDE_ENGINE_GRAPHICS_ECS_LIGHTING_SYSTEM_H_
