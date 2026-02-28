#include "graphics/shader.h"

#include <glad/glad.h>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>
#include <memory>
#include <string>

namespace engine::graphics {

std::unique_ptr<Shader> Shader::create_from_source(
    const std::string& vertex_source, const std::string& fragment_source) {
  // 1. Create and compile Vertex Shader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* v_source = vertex_source.c_str();
  glShaderSource(vertex_shader, 1, &v_source, 0);
  glCompileShader(vertex_shader);

  // print compile errors if any
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, 0, info_log);
    std::cout << "Vertex shader compilation failed:\n" << info_log << std::endl;
    return nullptr;
  };

  // 2. Create and compile Fragment Shader
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* f_source = fragment_source.c_str();
  glShaderSource(fragment_shader, 1, &f_source, nullptr);
  glCompileShader(fragment_shader);

  // Check for FS errors
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    std::cerr << "Fragment shader compilation failed:\n"
              << info_log << std::endl;
    return nullptr;
  }

  // 3. Link into a Program
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  // Check for Linking errors
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(program, 512, nullptr, info_log);
    std::cerr << "Shader program linking failed:\n" << info_log << std::endl;
    return nullptr;
  }

  // Clean up temporary shader objects
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return std::unique_ptr<Shader>(new Shader(program));
}

Shader::~Shader() { glDeleteProgram(shader_id_); }

void Shader::bind() const { glUseProgram(shader_id_); }

void Shader::unbind() const { glUseProgram(0); }

void Shader::set_int(const std::string& name, int value) {
  glUniform1i(get_uniform_location(name), value);
}

void Shader::set_vec3(const std::string& name, glm::vec3 value) {
  glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
}

void Shader::set_vec4(const std::string& name, glm::vec4 value) {
  glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
}

void Shader::set_mat4(const std::string& name, glm::mat4 value) {
  glUniformMatrix4fv(get_uniform_location(name), 1, false,
                     glm::value_ptr(value));
}

// Private functions

Shader::Shader(unsigned int shader_id) : shader_id_(shader_id) {}

int Shader::get_uniform_location(const std::string& name) const {
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
