#include <gtest/gtest.h>
#include <engine/input/input_manager.h>

namespace engine {

class InputManagerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Reset or initialize InputManager if necessary
  }
};

TEST_F(InputManagerTest, HandleResizeAndCursorPosition) {
  InputManager& input = InputManager::Get();

  // Standard 720p window
  input.HandleResize(1280, 720, 1.0f, 1.0f);

  // Cursor at (100, 100) in screen coordinates
  input.HandleCursorPosition(100.0, 100.0);

  // In 1280x720, Y=100 from top is Y = 720 - 100 = 620 from bottom
  EXPECT_FLOAT_EQ(input.mouse_screen_pos().x, 100.0f);
  EXPECT_FLOAT_EQ(input.mouse_screen_pos().y, 620.0f);

  // Retina/High-DPI case: 2x scaling
  // Screen size 500x500, Framebuffer 1000x1000
  input.HandleResize(1000, 1000, 2.0f, 2.0f);

  // Cursor at (100, 100) in screen coordinates
  input.HandleCursorPosition(100.0, 100.0);

  // Scaled X = 100 * 2 = 200
  // Scaled Y = 100 * 2 = 200
  // In 1000x1000, Y=200 from top is Y = 1000 - 200 = 800 from bottom
  EXPECT_FLOAT_EQ(input.mouse_screen_pos().x, 200.0f);
  EXPECT_FLOAT_EQ(input.mouse_screen_pos().y, 800.0f);
}

} // namespace engine
