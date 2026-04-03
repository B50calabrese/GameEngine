/**
 * @file console.cpp
 * @brief Implementation of the developer console.
 */

#include <algorithm>
#include <sstream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <engine/core/engine.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/util/console.h>
#include <engine/util/scripting/script_manager.h>

namespace engine::util {

Console& Console::Get() {
  static Console instance;
  return instance;
}

Console::Console() {
  RegisterCommand("clear", [this](const std::vector<std::string>&) {
    output_history_.clear();
  });
  RegisterCommand("help", [this](const std::vector<std::string>&) {
    Log("Available commands:");
    for (const auto& [name, _] : commands_) {
      Log("  " + name);
    }
  });
  RegisterCommand("pause", [this](const std::vector<std::string>&) {
    paused_ = !paused_;
    Log(paused_ ? "Game paused" : "Game unpaused");
  });
  RegisterCommand("exit", [](const std::vector<std::string>&) {
    glfwSetWindowShouldClose(Engine::window().native_handle(), GLFW_TRUE);
  });

  Log("Developer Console Initialized. Type 'help' for commands.");
}

void Console::Toggle() {
  visible_ = !visible_;
  if (visible_) {
    // Clear the tilde from the buffer if it was just pressed
    InputManager::Get().ClearTextInput();
  }
}

void Console::RegisterCommand(const std::string& name,
                              CommandCallback callback) {
  commands_[name] = callback;
}

void Console::Log(const std::string& message) {
  output_history_.push_back(message);
  if (output_history_.size() > 50) {
    output_history_.erase(output_history_.begin());
  }
}

void Console::Update() {
  if (!visible_) return;

  auto& input = InputManager::Get();

  // Handle Backspace
  if (input.IsKeyPressed(KeyCode::kBackspace)) {
    if (!input_buffer_.empty()) {
      input_buffer_.pop_back();
    }
  }

  // Handle Enter
  if (input.IsKeyPressed(KeyCode::kEnter)) {
    if (!input_buffer_.empty()) {
      Log("> " + input_buffer_);
      Execute(input_buffer_);
      input_buffer_.clear();
    }
  }

  // Handle Escape (Close console)
  if (input.IsKeyPressed(KeyCode::kEscape)) {
    Toggle();
    return;
  }

  // Handle character input
  const std::string& text = input.text_input();
  for (char c : text) {
    if (c >= 32 && c <= 126 &&
        c != '`') {  // Ignore tilde/backtick if it was used to open
      input_buffer_ += c;
    }
  }
  input.ClearTextInput();
}

void Console::Execute(const std::string& line) {
  std::stringstream ss(line);
  std::string cmd;
  ss >> cmd;

  auto it = commands_.find(cmd);
  if (it != commands_.end()) {
    std::vector<std::string> args;
    std::string arg;
    while (ss >> arg) {
      args.push_back(arg);
    }
    it->second(args);
  } else {
    // Try Lua
    sol::state& lua = ScriptManager::Get().state();
    auto result = lua.safe_script(line, sol::script_pass_on_error);
    if (!result.valid()) {
      sol::error err = result;
      Log("Error: " + std::string(err.what()));
    }
  }
}

void Console::Render() {
  if (!visible_) return;

  auto& renderer = graphics::Renderer::Get();
  int width = Engine::window().width();
  int height = Engine::window().height();

  float console_height = height * 0.4f;

  // Background
  renderer.DrawQuad({0, height - console_height},
                    {(float)width, console_height}, {0.0f, 0.0f, 0.0f, 1.0f});
  // Semi-transparent overlay
  renderer.DrawQuad({0, height - console_height},
                    {(float)width, console_height}, {0.1f, 0.1f, 0.1f, 0.8f});

  float x = 10.0f;
  float y = height - console_height + 10.0f;
  float line_height = 20.0f;

  // Input line
  renderer.DrawText("default", "> " + input_buffer_ + "_", {x, y}, 0.0f, 1.0f,
                    {1.0f, 1.0f, 1.0f, 1.0f});

  // History (bottom to top from above input)
  y += line_height;
  for (int i = (int)output_history_.size() - 1; i >= 0; --i) {
    renderer.DrawText("default", output_history_[i], {x, y}, 0.0f, 0.7f,
                      {0.8f, 0.8f, 0.8f, 1.0f});
    y += line_height * 0.7f;
    if (y > height) break;
  }
}

}  // namespace engine::util
