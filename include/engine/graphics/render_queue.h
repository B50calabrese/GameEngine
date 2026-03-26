/**
 * @file render_queue.h
 * @dir include/engine/graphics
 * @brief Sorting and batching of render commands.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_RENDER_QUEUE_H_
#define INCLUDE_ENGINE_GRAPHICS_RENDER_QUEUE_H_

#include <algorithm>
#include <vector>

#include <glm/glm.hpp>

#include <engine/graphics/primitive_renderer.h>
#include <engine/graphics/renderer.h>

namespace engine::graphics {

/** @brief Represents a single drawing command stored in the queue. */
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
 * @brief A high-level utility to sort and optimize draw calls
 * before sending them to the core Renderer.
 */
class RenderQueue {
 public:
  /**
   * @brief Returns the default global RenderQueue instance.
   * @return Reference to the default RenderQueue.
   */
  static RenderQueue& Default() {
    static RenderQueue instance;
    return instance;
  }

  /** @brief Adds a command to the queue without executing it immediately. */
  void Submit(const RenderCommand& command) { commands_.push_back(command); }

  /**
   * @brief Sorts commands by Z-order and TextureID, then executes them
   * via the singleton Renderer.
   */
  void Flush() {
    // Stable sort to preserve submission order for identical Z/Texture
    std::stable_sort(commands_.begin(), commands_.end(),
                     [](const RenderCommand& a, const RenderCommand& b) {
                       if (a.z_order != b.z_order) {
                         return a.z_order < b.z_order;
                       }
                       return a.texture_id < b.texture_id;
                     });

    for (const auto& cmd : commands_) {
      PrimitiveRenderer::SubmitTexturedQuad(cmd.position, cmd.size,
                                            cmd.texture_id, cmd.uv_min,
                                            cmd.uv_max, cmd.color, cmd.rotation,
                                            cmd.origin, cmd.is_font);
    }
    Clear();
  }

  /** @brief Clears the queue for the next frame. */
  void Clear() { commands_.clear(); }

 private:
  std::vector<RenderCommand> commands_;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_RENDER_QUEUE_H_
