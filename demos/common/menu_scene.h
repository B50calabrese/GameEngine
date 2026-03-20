/**
 * @file menu_scene.h
 * @brief Base menu scene for game demos.
 */

#ifndef DEMOS_COMMON_MENU_SCENE_H_
#define DEMOS_COMMON_MENU_SCENE_H_

#include <engine/scene/scene.h>
#include <string>
#include <vector>
#include <functional>
#include <glm/vec4.hpp>

namespace demos::common {

/** @brief Simple reusable menu scene. */
class BaseMenuScene : public engine::Scene {
 public:
  struct MenuItem {
    std::string text;
    std::function<void()> action;
  };

  BaseMenuScene(const std::string& title, const std::vector<MenuItem>& items);

  void OnAttach() override;
  void OnUpdate(float dt) override;
  void OnRender() override;

 private:
  std::string title_;
  std::vector<MenuItem> items_;
  int selected_index_ = 0;
  glm::vec4 title_color_ = {0.0f, 1.0f, 0.8f, 1.0f};
  glm::vec4 item_color_ = {1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 selected_color_ = {1.0f, 1.0f, 0.0f, 1.0f};
};

}  // namespace demos::common

#endif  // DEMOS_COMMON_MENU_SCENE_H_
