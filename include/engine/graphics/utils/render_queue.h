/**
 * @file render_queue.h
 * @brief Sorting and batching of render commands.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_UTILS_RENDER_QUEUE_H_
#define INCLUDE_ENGINE_GRAPHICS_UTILS_RENDER_QUEUE_H_

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

#include <engine/graphics/primitive_renderer.h>

namespace engine::graphics::utils {

/** @brief A single drawing command. */
struct RenderCommand {
  float z_order = 0.0f;
  unsigned int texture_id = 0;
  glm::vec2 position;
  glm::vec2 size;
  glm::vec4 color = glm::vec4(1.0f);
  float rotation = 0.0f;
  glm::vec2 uv_min = {0.0f, 0.0f};
  glm::vec2 uv_max = {1.0f, 1.0f};
  glm::vec2 origin = {0.0f, 0.0f};
  bool is_font = false;
};

/**
 * @brief Sorts and optimizes draw calls.
 */
class RenderQueue {
 public:
  /** @brief Gets the default global instance. */
  static RenderQueue& Default() {
    static RenderQueue instance;
    return instance;
  }

  /** @brief Submits a command. */
  void Submit(const RenderCommand& command) { commands_.push_back(command); }

  /** @brief Sorts and executes commands. */
  void Flush() {
    std::stable_sort(commands_.begin(), commands_.end(),
                     [](const RenderCommand& a, const RenderCommand& b) {
                       if (a.z_order != b.z_order) {
                         return a.z_order < b.z_order;
                       }
                       return a.texture_id < b.texture_id;
                     });

    for (const auto& cmd : commands_) {
      PrimitiveRenderer::SubmitTexturedQuad(
          cmd.position, cmd.size, cmd.texture_id, cmd.uv_min, cmd.uv_max,
          cmd.color, cmd.rotation, cmd.origin, cmd.is_font);
    }
    Clear();
  }

  /** @brief Clears the queue. */
  void Clear() { commands_.clear(); }

 private:
  RenderQueue() = default;
  std::vector<RenderCommand> commands_;
};

}  // namespace engine::graphics::utils

#endif  // INCLUDE_ENGINE_GRAPHICS_UTILS_RENDER_QUEUE_H_
