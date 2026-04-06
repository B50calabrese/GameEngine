/**
 * @file scene_manager.cpp
 * @brief SceneManager class implementation.
 */

#include <memory>

#include <engine/scene/scene_manager.h>

namespace engine {
void SceneManager::SetScene(std::unique_ptr<Scene> new_scene) {
  pending_changes_.push_back({SceneAction::kSet, std::move(new_scene)});
}

void SceneManager::PushScene(std::unique_ptr<Scene> overlay) {
  pending_changes_.push_back({SceneAction::kPush, std::move(overlay)});
}

void SceneManager::PopScene() {
  pending_changes_.push_back({SceneAction::kPop, nullptr});
}

void SceneManager::ProcessPendingChanges() {
  if (pending_changes_.empty()) {
    return;
  }

  // Move pending changes to a local vector to avoid issues if a scene change
  // is requested during the processing (e.g., in OnAttach or OnDetach).
  std::vector<PendingChange> changes = std::move(pending_changes_);
  pending_changes_.clear();

  for (auto& change : changes) {
    switch (change.action) {
      case SceneAction::kSet: {
        for (auto it = scene_stack_.rbegin(); it != scene_stack_.rend(); ++it) {
          (*it)->OnDetach();
        }
        scene_stack_.clear();
        if (change.scene) {
          scene_stack_.push_back(std::move(change.scene));
          scene_stack_.back()->OnAttach();
        }
        break;
      }
      case SceneAction::kPush: {
        if (change.scene) {
          scene_stack_.push_back(std::move(change.scene));
          scene_stack_.back()->OnAttach();
        }
        break;
      }
      case SceneAction::kPop: {
        if (!scene_stack_.empty()) {
          scene_stack_.back()->OnDetach();
          scene_stack_.pop_back();
        }
        break;
      }
    }
  }
}

void SceneManager::UpdateActiveScene(float delta_time) {
  if (!scene_stack_.empty()) {
    scene_stack_.back()->OnUpdate(delta_time);
  }
}

void SceneManager::RenderActiveScene() {
  for (auto& scene : scene_stack_) {
    scene->OnRender();
  }
}

bool SceneManager::DispatchInput() {
  if (scene_stack_.empty()) {
    return false;
  }
  return scene_stack_.back()->OnInput();
}

Scene* SceneManager::GetActiveScene() {
  if (scene_stack_.empty()) {
    return nullptr;
  }
  return scene_stack_.back().get();
}

}  // namespace engine
