#include <leveleditor/level_editor.h>
#include <leveleditor/component_registry.h>
#include <engine/ecs/components/transform.h>
#include <engine/util/console.h>
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace leveleditor {

LevelEditor::LevelEditor(engine::ecs::Registry& registry) : registry_(registry) {
  ComponentRegistry::Get().Init();
}

void LevelEditor::Render() {
  RenderToolbar();
  RenderEntityList();
  RenderInspector();
}

void LevelEditor::RenderToolbar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ImGui::InputText("Level Name", level_name_buffer_, sizeof(level_name_buffer_));
      if (ImGui::MenuItem("Save Level")) {
        SaveLevel(level_name_buffer_);
      }
      if (ImGui::MenuItem("Load Level")) {
        LoadLevel(level_name_buffer_);
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Playtest")) {
        bool is_paused = engine::util::Console::Get().IsPaused();
        if (ImGui::MenuItem("Play", nullptr, !is_paused)) {
            engine::util::Console::Get().SetPaused(false);
        }
        if (ImGui::MenuItem("Pause", nullptr, is_paused)) {
            engine::util::Console::Get().SetPaused(true);
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void LevelEditor::RenderEntityList() {
  ImGui::Begin("Entities");

  if (ImGui::Button("Add Entity")) {
    auto entity = registry_.CreateEntity();
    registry_.AddComponent(entity, engine::ecs::components::Transform{});
    selected_entity_ = entity;
    entity_selected_ = true;
  }

  ImGui::Separator();

  auto entities = registry_.GetView<engine::ecs::components::Transform>();
  for (auto entity : entities) {
    std::string label = "Entity " + std::to_string(entity);
    if (ImGui::Selectable(label.c_str(), entity_selected_ && selected_entity_ == entity)) {
      selected_entity_ = entity;
      entity_selected_ = true;
    }
  }

  ImGui::End();
}

void LevelEditor::RenderInspector() {
  ImGui::Begin("Inspector");

  if (entity_selected_ && registry_.IsAlive(selected_entity_)) {
    auto& components = ComponentRegistry::Get().GetComponents();

    for (auto& [name, info] : components) {
      if (info.has_func(selected_entity_, registry_)) {
        if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
          info.ui_func(selected_entity_, registry_);

          if (name != "Transform") { // Don't remove transform for now as it is the base
            if (ImGui::Button(("Remove " + name).c_str())) {
              info.remove_func(selected_entity_, registry_);
            }
          }
        }
      }
    }

    ImGui::Separator();
    if (ImGui::Button("Add Component")) {
      ImGui::OpenPopup("AddComponentPopup");
    }

    if (ImGui::BeginPopup("AddComponentPopup")) {
      for (auto& [name, info] : components) {
        if (!info.has_func(selected_entity_, registry_)) {
          if (ImGui::MenuItem(name.c_str())) {
            info.add_func(selected_entity_, registry_);
          }
        }
      }
      ImGui::EndPopup();
    }

    if (ImGui::Button("Delete Entity")) {
        registry_.DeleteEntity(selected_entity_);
        entity_selected_ = false;
    }
  } else {
    ImGui::Text("Select an entity to view its components.");
  }

  ImGui::End();
}

void LevelEditor::SaveLevel(const std::string& filename) {
  nlohmann::json root = nlohmann::json::array();
  auto& comp_registry = ComponentRegistry::Get();

  auto entities = registry_.GetView<engine::ecs::components::Transform>();
  for (auto entity : entities) {
    nlohmann::json entity_json = nlohmann::json::object();
    for (auto& [name, info] : comp_registry.GetComponents()) {
      if (info.has_func(entity, registry_)) {
        nlohmann::json comp_json;
        info.serialize_func(entity, registry_, comp_json);
        entity_json[name] = comp_json;
      }
    }
    root.push_back(entity_json);
  }

  std::string filepath = filename;
  // If it's just a filename, try to save in assets/levels/
  if (!std::filesystem::path(filename).has_parent_path()) {
      std::filesystem::create_directories("demos/assets/levels");
      filepath = "demos/assets/levels/" + filename;
  }

  std::ofstream file(filepath);
  if (file.is_open()) {
    file << root.dump(4);
    std::cout << "Level saved to " << filepath << std::endl;
  }
}

void LevelEditor::LoadLevel(const std::string& filename) {
  std::string filepath = filename;
  if (!std::filesystem::path(filename).has_parent_path()) {
      filepath = "demos/assets/levels/" + filename;
  }

  std::ifstream file(filepath);
  if (!file.is_open()) return;

  nlohmann::json root;
  file >> root;

  registry_.Clear();
  entity_selected_ = false;

  auto& comp_registry = ComponentRegistry::Get();
  for (auto& entity_json : root) {
    auto entity = registry_.CreateEntity();
    for (auto it = entity_json.begin(); it != entity_json.end(); ++it) {
      std::string comp_name = it.key();
      if (comp_registry.GetComponents().count(comp_name)) {
        comp_registry.GetComponents().at(comp_name).deserialize_func(entity, registry_, it.value());
      }
    }
  }
  std::cout << "Level loaded from " << filepath << std::endl;
}

}  // namespace leveleditor
