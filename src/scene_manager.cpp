#include "scene_manager.h"

#include <memory>

namespace engine {
void SceneManager::SetScene(std::unique_ptr<Scene> newScene) {
  // Detach all existing scenes in reverse order (top to bottom)
  for (auto it = scene_stack.rbegin(); it != scene_stack.rend(); ++it) {
    (*it)->OnDetach();
  }
  scene_stack.clear();

  // Initialize and add the new scene
  if (newScene) {
    scene_stack.push_back(std::move(newScene));
    scene_stack.back()->OnAttach();
  }
}

void SceneManager::PushScene(std::unique_ptr<Scene> overlay) {
  if (overlay) {
    scene_stack.push_back(std::move(overlay));
    scene_stack.back()->OnAttach();
  }
}

void SceneManager::PopScene() {
  if (scene_stack.empty()) return;

  scene_stack.back()->OnDetach();
  scene_stack.pop_back();
}

// --- Private Dispatchers (Accessible only via Application) ---

void SceneManager::UpdateActiveScene(float delta_time) {
  if (!scene_stack.empty()) {
    // Usually, only the top scene processes logic updates
    scene_stack.back()->OnUpdate(delta_time);
  }
}

void SceneManager::RenderActiveScene() {
  // We iterate through the whole stack to render.
  // This allows lower scenes (like the game world) to be visible
  // under transparent overlays (like a Pause menu).
  for (auto& scene : scene_stack) {
    scene->OnRender();
  }
}

bool SceneManager::DispatchInput() {
  if (scene_stack.empty()) return false;

  // Dispatch input to the top-most scene first.
  // If it returns true, it "consumed" the input event.
  return scene_stack.back()->OnInput();
}
}  // namespace engine