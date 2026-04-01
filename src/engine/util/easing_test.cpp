#include <gtest/gtest.h>
#include <engine/util/easing.h>
#include <cmath>

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

TEST(EasingTest, InCubic) {
  EXPECT_FLOAT_EQ(Easing::InCubic(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::InCubic(0.5f), 0.125f);
  EXPECT_FLOAT_EQ(Easing::InCubic(1.0f), 1.0f);
}

TEST(EasingTest, OutCubic) {
  EXPECT_FLOAT_EQ(Easing::OutCubic(0.0f), 0.0f);
  EXPECT_NEAR(Easing::OutCubic(0.5f), 0.875f, 1e-6f);
  EXPECT_FLOAT_EQ(Easing::OutCubic(1.0f), 1.0f);
}

TEST(EasingTest, InSine) {
  EXPECT_FLOAT_EQ(Easing::InSine(0.0f), 0.0f);
  EXPECT_NEAR(Easing::InSine(0.5f), 1.0f - std::sqrt(2.0f) / 2.0f, 1e-6f);
  EXPECT_FLOAT_EQ(Easing::InSine(1.0f), 1.0f);
}

TEST(EasingTest, OutSine) {
  EXPECT_FLOAT_EQ(Easing::OutSine(0.0f), 0.0f);
  EXPECT_NEAR(Easing::OutSine(0.5f), std::sqrt(2.0f) / 2.0f, 1e-6f);
  EXPECT_FLOAT_EQ(Easing::OutSine(1.0f), 1.0f);
}

TEST(EasingTest, InBounce) {
  EXPECT_FLOAT_EQ(Easing::InBounce(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::InBounce(1.0f), 1.0f);
}

TEST(EasingTest, OutBounce) {
  EXPECT_FLOAT_EQ(Easing::OutBounce(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::OutBounce(1.0f), 1.0f);
}

TEST(EasingTest, InElastic) {
  EXPECT_FLOAT_EQ(Easing::InElastic(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::InElastic(1.0f), 1.0f);
}

TEST(EasingTest, OutElastic) {
  EXPECT_FLOAT_EQ(Easing::OutElastic(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::OutElastic(1.0f), 1.0f);
}

TEST(EasingTest, InBack) {
  EXPECT_FLOAT_EQ(Easing::InBack(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::InBack(1.0f), 1.0f);
}

TEST(EasingTest, OutBack) {
  EXPECT_FLOAT_EQ(Easing::OutBack(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Easing::OutBack(1.0f), 1.0f);
}

TEST(EasingTest, ApplyLinear) {
  EXPECT_FLOAT_EQ(Easing::Apply(EaseType::Linear, 0.5f), 0.5f);
}

TEST(EasingTest, ApplyInQuad) {
  EXPECT_FLOAT_EQ(Easing::Apply(EaseType::InQuad, 0.5f), 0.25f);
}

}  // namespace engine::util
