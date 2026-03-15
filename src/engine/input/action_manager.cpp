/**
 * @file action_manager.cpp
 * @brief ActionManager class implementation.
 */

#include <engine/input/action_manager.h>

namespace engine {

ActionManager& ActionManager::Get() {
  static ActionManager instance;
  return instance;
}

void ActionManager::BindAction(const std::string& action_name,
                               KeyCode key_code) {
  registry_[action_name].bindings.push_back(key_code);
}

void ActionManager::Update() {
  auto& im = InputManager::Get();
  for (auto& [name, data] : registry_) {
    data.was_active_last_frame = data.is_active_this_frame;
    data.is_active_this_frame = false;
    for (KeyCode code : data.bindings) {
      if (im.IsKeyDown(code)) {
        data.is_active_this_frame = true;
        break;
      }
    }
  }
}

bool ActionManager::IsStarted(const std::string& action_name) const {
  auto it = registry_.find(action_name);
  if (it == registry_.end()) return false;
  return it->second.is_active_this_frame && !it->second.was_active_last_frame;
}

bool ActionManager::IsOngoing(const std::string& action_name) const {
  auto it = registry_.find(action_name);
  if (it == registry_.end()) return false;
  return it->second.is_active_this_frame;
}

bool ActionManager::IsReleased(const std::string& action_name) const {
  auto it = registry_.find(action_name);
  if (it == registry_.end()) return false;
  return !it->second.is_active_this_frame && it->second.was_active_last_frame;
}

bool ActionManager::IsConsumed() const {
  return InputManager::Get().IsConsumed();
}

void ActionManager::Consume() { InputManager::Get().Consume(); }

}  // namespace engine
