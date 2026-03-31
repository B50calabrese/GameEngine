/**
 * @file scene_manager.cpp
 * @brief SceneManager class implementation.
 */

#include <memory>

#include <engine/scene/scene_manager.h>

namespace engine {

void SceneManager::SetScene(std::unique_ptr<Scene> new_scene) {
  while (!scene_stack_.empty()) {
    PopScene();
  }
  if (new_scene) {
    scene_stack_.push_back(std::move(new_scene));
    scene_stack_.back()->OnAttach();
  }
}

void SceneManager::PushScene(std::unique_ptr<Scene> overlay) {
  if (overlay) {
    scene_stack_.push_back(std::move(overlay));
    scene_stack_.back()->OnAttach();
  }
}

void SceneManager::PopScene() {
  if (!scene_stack_.empty()) {
    scene_stack_.back()->OnDetach();
    scene_stack_.pop_back();
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
