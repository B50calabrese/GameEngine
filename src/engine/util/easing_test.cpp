#include <gtest/gtest.h>
#include <engine/util/easing.h>

namespace engine::util {

TEST(EasingTest, Linear) {
  EXPECT_FLOAT_EQ(Easing::Linear(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::Linear(0.5f), 0.5f);
  EXPECT_FLOAT_EQ(Easing::Linear(1.0f), 1.0f);
}

TEST(EasingTest, InQuad) {
  EXPECT_FLOAT_EQ(Easing::InQuad(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::InQuad(0.5f), 0.25f);
  EXPECT_FLOAT_EQ(Easing::InQuad(1.0f), 1.0f);
}

TEST(EasingTest, OutQuad) {
  EXPECT_FLOAT_EQ(Easing::OutQuad(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::OutQuad(0.5f), 0.75f);
  EXPECT_FLOAT_EQ(Easing::OutQuad(1.0f), 1.0f);
}

TEST(EasingTest, ApplyLinear) {
  EXPECT_FLOAT_EQ(Easing::Apply(EaseType::Linear, 0.5f), 0.5f);
}

TEST(EasingTest, ApplyInQuad) {
  EXPECT_FLOAT_EQ(Easing::Apply(EaseType::InQuad, 0.5f), 0.25f);
}

}  // namespace engine::util
