#include <imgui.h>
#include <leveleditor/component_registry.h>

#include <cstring>

#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/physics_components.h>
#include <engine/ecs/components/transform.h>

namespace leveleditor {

using namespace engine::ecs::components;

ComponentRegistry& ComponentRegistry::Get() {
  static ComponentRegistry instance;
  return instance;
}

void ComponentRegistry::RegisterComponent(const ComponentInfo& info) {
  components_[info.name] = info;
}

void ComponentRegistry::Init() {
  // --- Transform ---
  RegisterComponent(
      {"Transform",
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         auto& t = reg.GetComponent<Transform>(entity);
         ImGui::DragFloat2("Position", &t.position.x, 0.1f);
         ImGui::DragFloat2("Scale", &t.scale.x, 0.1f);
         ImGui::DragFloat("Rotation", &t.rotation, 0.1f);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg,
          nlohmann::json& j) {
         auto& t = reg.GetComponent<Transform>(entity);
         j["position"] = {t.position.x, t.position.y};
         j["scale"] = {t.scale.x, t.scale.y};
         j["rotation"] = t.rotation;
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg,
          const nlohmann::json& j) {
         Transform t;
         t.position = {j["position"][0], j["position"][1]};
         t.scale = {j["scale"][0], j["scale"][1]};
         t.rotation = j["rotation"];
         reg.AddComponent(entity, t);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         reg.AddComponent(entity, Transform{});
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         reg.RemoveComponent<Transform>(entity);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         return reg.HasComponent<Transform>(entity);
       }});

  // --- Sprite ---
  RegisterComponent(
      {"Sprite",
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         auto& s = reg.GetComponent<Sprite>(entity);
         char buf[128];
         strncpy(buf, s.texture_name.c_str(), sizeof(buf));
         if (ImGui::InputText("Texture", buf, sizeof(buf)))
           s.texture_name = buf;

         strncpy(buf, s.sprite_sheet_name.c_str(), sizeof(buf));
         if (ImGui::InputText("Sheet", buf, sizeof(buf)))
           s.sprite_sheet_name = buf;

         ImGui::DragInt("Index", &s.sprite_index);
         ImGui::ColorEdit4("Tint", &s.tint.r);
         ImGui::DragFloat2("Origin", &s.origin.x, 0.1f);
         ImGui::DragFloat("Z-Index", &s.z_index, 0.1f);
         ImGui::Checkbox("Visible", &s.visible);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg,
          nlohmann::json& j) {
         auto& s = reg.GetComponent<Sprite>(entity);
         j["texture_name"] = s.texture_name;
         j["sprite_sheet_name"] = s.sprite_sheet_name;
         j["sprite_index"] = s.sprite_index;
         j["tint"] = {s.tint.r, s.tint.g, s.tint.b, s.tint.a};
         j["origin"] = {s.origin.x, s.origin.y};
         j["z_index"] = s.z_index;
         j["visible"] = s.visible;
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg,
          const nlohmann::json& j) {
         Sprite s;
         s.texture_name = j["texture_name"];
         s.sprite_sheet_name = j["sprite_sheet_name"];
         s.sprite_index = j["sprite_index"];
         s.tint = {j["tint"][0], j["tint"][1], j["tint"][2], j["tint"][3]};
         s.origin = {j["origin"][0], j["origin"][1]};
         s.z_index = j["z_index"];
         s.visible = j["visible"];
         reg.AddComponent(entity, s);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         reg.AddComponent(entity, Sprite{});
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         reg.RemoveComponent<Sprite>(entity);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         return reg.HasComponent<Sprite>(entity);
       }});

  // --- Quad ---
  RegisterComponent(
      {"Quad",
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         auto& q = reg.GetComponent<Quad>(entity);
         ImGui::ColorEdit4("Color", &q.color.r);
         ImGui::DragFloat("Z-Index", &q.z_index, 0.1f);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg,
          nlohmann::json& j) {
         auto& q = reg.GetComponent<Quad>(entity);
         j["color"] = {q.color.r, q.color.g, q.color.b, q.color.a};
         j["z_index"] = q.z_index;
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg,
          const nlohmann::json& j) {
         Quad q;
         q.color = {j["color"][0], j["color"][1], j["color"][2], j["color"][3]};
         q.z_index = j["z_index"];
         reg.AddComponent(entity, q);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         reg.AddComponent(entity, Quad{});
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         reg.RemoveComponent<Quad>(entity);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         return reg.HasComponent<Quad>(entity);
       }});

  // --- Physics (Collider) ---
  RegisterComponent(
      {"Collider",
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         auto& c = reg.GetComponent<Collider>(entity);
         ImGui::DragFloat2("Size", &c.size.x, 0.1f);
         ImGui::DragFloat2("Offset", &c.offset.x, 0.1f);
         ImGui::Checkbox("Is Trigger", &c.is_trigger);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg,
          nlohmann::json& j) {
         auto& c = reg.GetComponent<Collider>(entity);
         j["size"] = {c.size.x, c.size.y};
         j["offset"] = {c.offset.x, c.offset.y};
         j["is_trigger"] = c.is_trigger;
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg,
          const nlohmann::json& j) {
         Collider c;
         c.size = {j["size"][0], j["size"][1]};
         c.offset = {j["offset"][0], j["offset"][1]};
         c.is_trigger = j["is_trigger"];
         reg.AddComponent(entity, c);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         reg.AddComponent(entity, Collider{});
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         reg.RemoveComponent<Collider>(entity);
       },
       [](engine::ecs::EntityID entity, engine::ecs::Registry& reg) {
         return reg.HasComponent<Collider>(entity);
       }});
}

}  // namespace leveleditor
