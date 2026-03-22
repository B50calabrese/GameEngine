#include <glm/glm.hpp>

#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>
#include <engine/util/scripting/script_manager.h>

namespace engine::util {

void ScriptManager::Init() {
  if (initialized_) {
    return;
  }

  lua_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math,
                      sol::lib::table, sol::lib::string);

  // Bind Logger
  auto log = lua_["Log"].get_or_create<sol::table>();
  log["info"] = [](const std::string& msg) { LOG_INFO("%s", msg.c_str()); };
  log["warn"] = [](const std::string& msg) { LOG_WARN("%s", msg.c_str()); };
  log["err"] = [](const std::string& msg) { LOG_ERR("%s", msg.c_str()); };

  // Bind GLM types
  lua_.new_usertype<glm::vec2>(
      "vec2", sol::constructors<glm::vec2(), glm::vec2(float, float)>(), "x",
      &glm::vec2::x, "y", &glm::vec2::y, "r", &glm::vec2::x, "g", &glm::vec2::y,
      sol::meta_function::addition,
      [](const glm::vec2& a, const glm::vec2& b) { return a + b; },
      sol::meta_function::subtraction,
      [](const glm::vec2& a, const glm::vec2& b) { return a - b; },
      sol::meta_function::multiplication,
      [](const glm::vec2& a, float b) { return a * b; },
      sol::meta_function::division,
      [](const glm::vec2& a, float b) { return a / b; });

  lua_.new_usertype<glm::vec3>(
      "vec3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
      "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z, "r",
      &glm::vec3::x, "g", &glm::vec3::y, "b", &glm::vec3::z,
      sol::meta_function::addition,
      [](const glm::vec3& a, const glm::vec3& b) { return a + b; },
      sol::meta_function::subtraction,
      [](const glm::vec3& a, const glm::vec3& b) { return a - b; },
      sol::meta_function::multiplication,
      [](const glm::vec3& a, float b) { return a * b; },
      sol::meta_function::division,
      [](const glm::vec3& a, float b) { return a / b; });

  lua_.new_usertype<glm::vec4>(
      "vec4",
      sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
      "x", &glm::vec4::x, "y", &glm::vec4::y, "z", &glm::vec4::z, "w",
      &glm::vec4::w, "r", &glm::vec4::x, "g", &glm::vec4::y, "b", &glm::vec4::z,
      "a", &glm::vec4::w, sol::meta_function::addition,
      [](const glm::vec4& a, const glm::vec4& b) { return a + b; },
      sol::meta_function::subtraction,
      [](const glm::vec4& a, const glm::vec4& b) { return a - b; },
      sol::meta_function::multiplication,
      [](const glm::vec4& a, float b) { return a * b; },
      sol::meta_function::division,
      [](const glm::vec4& a, float b) { return a / b; });

  // Bind InputManager
  lua_.new_enum<KeyCode>("KeyCode", {{"MouseLeft", KeyCode::kMouseLeft},
                                     {"MouseRight", KeyCode::kMouseRight},
                                     {"MouseMiddle", KeyCode::kMouseMiddle},
                                     {"A", KeyCode::kA},
                                     {"B", KeyCode::kB},
                                     {"C", KeyCode::kC},
                                     {"D", KeyCode::kD},
                                     {"E", KeyCode::kE},
                                     {"F", KeyCode::kF},
                                     {"G", KeyCode::kG},
                                     {"H", KeyCode::kH},
                                     {"I", KeyCode::kI},
                                     {"J", KeyCode::kJ},
                                     {"K", KeyCode::kK},
                                     {"L", KeyCode::kL},
                                     {"M", KeyCode::kM},
                                     {"N", KeyCode::kN},
                                     {"O", KeyCode::kO},
                                     {"P", KeyCode::kP},
                                     {"Q", KeyCode::kQ},
                                     {"R", KeyCode::kR},
                                     {"S", KeyCode::kS},
                                     {"T", KeyCode::kT},
                                     {"U", KeyCode::kU},
                                     {"V", KeyCode::kV},
                                     {"W", KeyCode::kW},
                                     {"X", KeyCode::kX},
                                     {"Y", KeyCode::kY},
                                     {"Z", KeyCode::kZ},
                                     {"0", KeyCode::k0},
                                     {"1", KeyCode::k1},
                                     {"2", KeyCode::k2},
                                     {"3", KeyCode::k3},
                                     {"4", KeyCode::k4},
                                     {"5", KeyCode::k5},
                                     {"6", KeyCode::k6},
                                     {"7", KeyCode::k7},
                                     {"8", KeyCode::k8},
                                     {"9", KeyCode::k9},
                                     {"Escape", KeyCode::kEscape},
                                     {"Enter", KeyCode::kEnter},
                                     {"Space", KeyCode::kSpace},
                                     {"Tab", KeyCode::kTab},
                                     {"LeftShift", KeyCode::kLeftShift},
                                     {"LeftControl", KeyCode::kLeftControl},
                                     {"LeftAlt", KeyCode::kLeftAlt},
                                     {"Backspace", KeyCode::kBackspace},
                                     {"Delete", KeyCode::kDelete},
                                     {"Up", KeyCode::kUp},
                                     {"Down", KeyCode::kDown},
                                     {"Left", KeyCode::kLeft},
                                     {"Right", KeyCode::kRight}});

  auto input = lua_["Input"].get_or_create<sol::table>();
  input["is_key_down"] = [](KeyCode key) {
    return InputManager::Get().IsKeyDown(key);
  };
  input["is_key_pressed"] = [](KeyCode key) {
    return InputManager::Get().IsKeyPressed(key);
  };
  input["is_key_released"] = [](KeyCode key) {
    return InputManager::Get().IsKeyReleased(key);
  };
  input["mouse_pos"] = []() { return InputManager::Get().mouse_screen_pos(); };

  // Bind SceneManager
  auto scene = lua_["Scene"].get_or_create<sol::table>();
  scene["pop"] = []() { SceneManager::Get().PopScene(); };
  // Note: SetScene and PushScene are harder because they take a unique_ptr<Scene>
  // which Lua cannot easily create without more complex bindings or a factory.
  // For now, let's just provide Pop.

  initialized_ = true;
}

bool ScriptManager::RunFile(const std::string& filepath) {
  auto result = lua_.script_file(filepath, sol::script_pass_on_error);
  if (!result.valid()) {
    sol::error err = result;
    LOG_ERR("Failed to run Lua script: %s\n%s", filepath.c_str(), err.what());
    return false;
  }
  return true;
}

}  // namespace engine::util
