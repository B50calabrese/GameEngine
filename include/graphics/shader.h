#ifndef INCLUDE_GRAPHICS_SHADER_H_
#define INCLUDE_GRAPHICS_SHADER_H_

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace engine::graphics {

/**
 * @brief Represents a compiled and linked GLSL shader program.
 *
 * This class handles the lifecycle of a shader, from creation to binding and
 * uniform setting.
 */
class Shader {
 public:
  /**
   * @brief Loads vertex and fragment shader source code, compiles them, and
   * links them into a single shader program.
   *
   * @param vertex_source A string containing the vertex shader source code.
   * @param fragment_source A string containing the fragment shader source code.
   * @return A unique pointer to the newly created `Shader` object, or nullptr.
   */
  static std::unique_ptr<Shader> CreateFromSource(
      const std::string& vertex_source, const std::string& fragment_source);

  /** @brief Deletes the shader program from the GPU. */
  ~Shader();

  /**
   * @brief Gets the OpenGL ID of the shader program.
   * @return The shader ID.
   */
  unsigned int id() const { return shader_id_; }

  /** @brief Activates this shader program for subsequent rendering calls. */
  void Bind() const;

  /** @brief Deactivates the currently bound shader program. */
  void Unbind() const;

  /**
   * @brief Sets an integer uniform variable in the shader.
   *
   * @param name The name of the uniform variable in the GLSL code.
   * @param value The integer value to set.
   */
  void SetInt(const std::string& name, int value);

  /**
   * @brief Sets a 3-component float vector uniform variable in the shader.
   *
   * @param name The name of the uniform variable in the GLSL code.
   * @param value The `glm::vec3` value to set.
   */
  void SetVec3(const std::string& name, glm::vec3 value);

  /**
   * @brief Sets a 4-component float vector uniform variable in the shader.
   *
   * @param name The name of the uniform variable in the GLSL code.
   * @param value The `glm::vec4` value to set.
   */
  void SetVec4(const std::string& name, glm::vec4 value);

  /**
   * @brief Sets a 4x4 matrix uniform variable in the shader.
   *
   * @param name The name of the uniform variable in the GLSL code.
   * @param value The `glm::mat4` value to set.
   */
  void SetMat4(const std::string& name, glm::mat4 value);

 private:
  explicit Shader(unsigned int shader_id);

  /**
   * @brief Retrieves the location of a uniform variable, using a cache for
   * performance.
   *
   * @param name The name of the uniform.
   * @return The location.
   */
  int get_uniform_location(const std::string& name) const;

  /** @brief Caching uniform locations improves performance significantly. */
  mutable std::unordered_map<std::string, int> uniform_location_cache_;
  unsigned int shader_id_;
};
}  // namespace engine::graphics

#endif  // INCLUDE_GRAPHICS_SHADER_H_
