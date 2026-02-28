#include "scene_manager.h"

#include <memory>

namespace engine {
void SceneManager::set_scene(std::unique_ptr<Scene> new_scene) {
  // Detach all existing scenes in reverse order (top to bottom)
  for (auto it = scene_stack_.rbegin(); it != scene_stack_.rend(); ++it) {
    (*it)->on_detach();
  }
  scene_stack_.clear();

  // Initialize and add the new scene
  if (new_scene) {
    scene_stack_.push_back(std::move(new_scene));
    scene_stack_.back()->on_attach();
  }
}

void SceneManager::push_scene(std::unique_ptr<Scene> overlay) {
  if (overlay) {
    scene_stack_.push_back(std::move(overlay));
    scene_stack_.back()->on_attach();
  }
}

void SceneManager::pop_scene() {
  if (scene_stack_.empty()) return;

  scene_stack_.back()->on_detach();
  scene_stack_.pop_back();
}

// --- Private Dispatchers (Accessible only via Application) ---

void SceneManager::update_active_scene(float delta_time) {
  if (!scene_stack_.empty()) {
    // Usually, only the top scene processes logic updates
    scene_stack_.back()->on_update(delta_time);
  }
}

void SceneManager::render_active_scene() {
  // We iterate through the whole stack to render.
  // This allows lower scenes (like the game world) to be visible
  // under transparent overlays (like a Pause menu).
  for (auto& scene : scene_stack_) {
    scene->on_render();
  }
}

bool SceneManager::dispatch_input() {
  if (scene_stack_.empty()) return false;

  // Dispatch input to the top-most scene first.
  // If it returns true, it "consumed" the input event.
  return scene_stack_.back()->on_input();
}
}  // namespace engine
