/**
 * @file sync_system.h
 * @brief System for syncing UI data bindings.
 */

#ifndef INCLUDE_ENGINE_UI_SYNC_SYSTEM_H_
#define INCLUDE_ENGINE_UI_SYNC_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace engine::ui {

/**
 * @brief Handles reactive data binding for UI elements.
 */
class UiSyncSystem {
 public:
  static void Update(ecs::Registry& reg);
};

}  // namespace engine::ui

#endif  // INCLUDE_ENGINE_UI_SYNC_SYSTEM_H_
