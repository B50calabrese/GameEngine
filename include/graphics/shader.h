#ifndef SHADER_H
#define SHADER_H

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <string>
#include <unordered_map>

namespace engine::graphics {

class Shader {
 public:
  /**
   * Loads and compiles a shader program from given vertex and fragment shader
   * source code.
   */
  static Shader* CreateFromSource(const std::string& vertexSource,
                                  const std::string& fragmentSource);
  ~Shader();

  /**
   * Activates the shader to be used.
   */
  void Bind() const;

  void Unbind() const;

  // These allow you to pass data from C++ to your GLSL code
  void SetInt(const std::string& name, int value);
  void SetVec3(const std::string& name, glm::vec3 value);
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