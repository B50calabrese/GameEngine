#ifndef INCLUDE_LEVELEDITOR_LEVEL_EDITOR_H_
#define INCLUDE_LEVELEDITOR_LEVEL_EDITOR_H_

#include <string>

#include <engine/ecs/registry.h>

namespace leveleditor {

/**
 * @brief Main UI and logic for the Level Editor toolkit.
 */
class LevelEditor {
 public:
  LevelEditor(engine::ecs::Registry& registry);

  /**
   * @brief Renders the Level Editor UI.
   */
  void Render();

  /**
   * @brief Saves the current level to a JSON file.
   */
  void SaveLevel(const std::string& filename);

  /**
   * @brief Loads a level from a JSON file.
   */
  void LoadLevel(const std::string& filename);

 private:
  void RenderEntityList();
  void RenderInspector();
  void RenderToolbar();

  engine::ecs::Registry& registry_;
  engine::ecs::EntityID selected_entity_;
  bool entity_selected_ = false;
  bool is_playtesting_ = false;
  char level_name_buffer_[128] = "level.json";
};

}  // namespace leveleditor

#endif  // INCLUDE_LEVELEDITOR_LEVEL_EDITOR_H_
