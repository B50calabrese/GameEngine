#ifndef SHADER_H
#define SHADER_H

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>
#include <unordered_map>

namespace engine::graphics {

// Represents a compiled and linked GLSL shader program. This class handles
// the lifecycle of a shader, from creation to binding and uniform setting.
class Shader {
 public:
  // Loads vertex and fragment shader source code, compiles them, and links them
  // into a single shader program.
  //
  // @param vertexSource A string containing the vertex shader source code.
  // @param fragmentSource A string containing the fragment shader source code.
  // @return A pointer to the newly created `Shader` object. The caller is
  // responsible for deleting this pointer.
  static Shader* CreateFromSource(const std::string& vertexSource,
                                  const std::string& fragmentSource);
  // Deletes the shader program from the GPU.
  ~Shader();

  // Activates this shader program for subsequent rendering calls.
  void Bind() const;

  // Deactivates the currently bound shader program.
  void Unbind() const;

  // Sets an integer uniform variable in the shader.
  //
  // @param name The name of the uniform variable in the GLSL code.
  // @param value The integer value to set.
  void SetInt(const std::string& name, int value);

  // Sets a 3-component float vector uniform variable in the shader.
  //
  // @param name The name of the uniform variable in the GLSL code.
  // @param value The `glm::vec3` value to set.
  void SetVec3(const std::string& name, glm::vec3 value);

  // Sets a 4-component float vector uniform variable in the shader.
  //
  // @param name The name of the uniform variable in the GLSL code.
  // @param value The `glm::vec4` value to set.
  void SetVec4(const std::string& name, glm::vec4 value);

 private:
  Shader(unsigned int shader_id);

  int GetUniformLocation(const std::string& name) const;

  // Caching uniform locations improves performance significantly
  mutable std::unordered_map<std::string, int> uniform_location_cache;
  unsigned int shader_id_;
};
}  // namespace engine::graphics

#endif  // SHADER_H