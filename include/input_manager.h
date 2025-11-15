#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <map>

class Window;

namespace engine {

enum class KeyCode {
  // --- Mouse Buttons ---
  KC_MOUSE_LEFT = 0,
  KC_MOUSE_RIGHT = 1,
  KC_MOUSE_MIDDLE = 2,

  // --- Alphabet Keys (Essential for Debug/Cheats/Chat) ---
  KC_A,
  KC_B,
  KC_C,
  KC_D,
  KC_E,
  KC_F,
  KC_G,
  KC_H,
  KC_I,
  KC_J,
  KC_K,
  KC_L,
  KC_M,
  KC_N,
  KC_O,
  KC_P,
  KC_Q,
  KC_R,
  KC_S,
  KC_T,
  KC_U,
  KC_V,
  KC_W,
  KC_X,
  KC_Y,
  KC_Z,

  // --- Number Keys (Top Row) ---
  KC_0,
  KC_1,
  KC_2,
  KC_3,
  KC_4,
  KC_5,
  KC_6,
  KC_7,
  KC_8,
  KC_9,

  // --- Functional Keys (Essential Game Controls) ---
  KC_ESCAPE,      // Crucial for menus, pausing, or quitting
  KC_ENTER,       // Confirming actions
  KC_SPACE,       // Common action key (e.g., end turn, skip dialog)
  KC_TAB,         // Toggling UI panels/map view
  KC_LEFT_SHIFT,  // Holding for multiple selection or speed
  KC_LEFT_CONTROL,
  KC_LEFT_ALT,
  KC_BACKSPACE,  // Text input cleanup
  KC_DELETE,

  // --- Navigation/Movement Keys (Even for 2D map movement) ---
  KC_UP,
  KC_DOWN,
  KC_LEFT,
  KC_RIGHT,

  // --- System Keys (Useful for debugging) ---
  KC_F1,
  KC_F2,
  KC_F3,
  KC_F4,
  KC_F5,
  KC_F6,
  KC_F7,
  KC_F8,
  KC_F9,
  KC_F10,
  KC_F11,
  KC_F12,

  // --- Utility Keys ---
  KC_HOME,
  KC_END,
  KC_PAGE_UP,
  KC_PAGE_DOWN,
};

class InputManager {
 public:
  static InputManager& Get();

  bool IsKeyDown(KeyCode key_code) const;

  bool IsKeyPressed(KeyCode key_code) const;

  bool IsKeyReleased(KeyCode key_code) const;

  float GetMouseX() const { return this->mouse_x; }

  float GetMouseY() const { return this->mouse_y; }

  void UpdateState();

  friend class Window;

 private:
  // Private Constructor/Deleters for Singleton pattern
  InputManager() = default;
  ~InputManager() = default;
  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;

  // Raw event handlers called by GLFW callbacks
  void HandleKey(int raw_key_code, int action);
  void HandleMouseButton(int raw_button_code, int action);
  void HandleCursorPosition(double xpos, double ypos);

  float mouse_x, mouse_y;
  std::map<KeyCode, bool> current_key_state;
  std::map<KeyCode, bool> previous_key_state;

  // Utility function to map GLFW codes to our internal KeyCode
  KeyCode MapRawCode(int raw_code) const;
};

}  // namespace engine

#endif  // INPUT_MANAGER_H