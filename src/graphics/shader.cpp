#include "graphics/shader.h"

#include <glad/glad.h>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <string>

namespace engine::graphics {

Shader* Shader::CreateFromSource(const std::string& vertexSource,
                                 const std::string& fragmentSource) {
  // 1. Create and compile Vertex Shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char* vSource = vertexSource.c_str();
  glShaderSource(vertexShader, 1, &vSource, 0);
  glCompileShader(vertexShader);

  // print compile errors if any
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
    std::cout << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    return nullptr;
  };

  // 2. Create and compile Fragment Shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fSource = fragmentSource.c_str();
  glShaderSource(fragmentShader, 1, &fSource, nullptr);
  glCompileShader(fragmentShader);

  // Check for FS errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cerr << "Fragment shader compilation failed:\n"
              << infoLog << std::endl;
    return nullptr;
  }

  // 3. Link into a Program
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  // Check for Linking errors
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    return nullptr;
  }

  // Clean up temporary shader objects
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return new Shader(program);
}

Shader::~Shader() { glDeleteProgram(shader_id_); }

void Shader::Bind() const { glUseProgram(shader_id_); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetInt(const std::string& name, int value) {
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) {
  glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string& name, glm::vec4 value) {
  glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(value));
}

// Private functions

Shader::Shader(unsigned int shader_id) : shader_id_(shader_id) {}

int Shader::GetUniformLocation(const std::string& name) const {
  if (uniform_location_cache_.find(name) != uniform_location_cache_.end())
    return uniform_location_cache_[name];

  int location = glGetUniformLocation(shader_id_, name.c_str());
  if (location == -1) {
    std::cout << "Warning: uniform '" << name
              << "' doesn't exist or is not used in shader!" << std::endl;
    return 0;
  }
  uniform_location_cache_[name] = location;
  return location;
}

}  // namespace engine::graphics
