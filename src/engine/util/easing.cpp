/**
 * @file easing.cpp
 * @brief Implementation of easing functions.
 */

#include <cmath>

#include <engine/util/easing.h>

namespace engine::util {

namespace {
constexpr float kPi = 3.1415926535f;
}

float Easing::Apply(EaseType type, float t) {
  switch (type) {
    case EaseType::Linear:
      return Linear(t);
    case EaseType::InQuad:
      return InQuad(t);
    case EaseType::OutQuad:
      return OutQuad(t);
    case EaseType::InOutQuad:
      return InOutQuad(t);
    case EaseType::InCubic:
      return InCubic(t);
    case EaseType::OutCubic:
      return OutCubic(t);
    case EaseType::InOutCubic:
      return InOutCubic(t);
    case EaseType::InSine:
      return InSine(t);
    case EaseType::OutSine:
      return OutSine(t);
    case EaseType::InOutSine:
      return InOutSine(t);
    case EaseType::InBounce:
      return InBounce(t);
    case EaseType::OutBounce:
      return OutBounce(t);
    case EaseType::InOutBounce:
      return InOutBounce(t);
    case EaseType::InElastic:
      return InElastic(t);
    case EaseType::OutElastic:
      return OutElastic(t);
    case EaseType::InOutElastic:
      return InOutElastic(t);
    case EaseType::InBack:
      return InBack(t);
    case EaseType::OutBack:
      return OutBack(t);
    case EaseType::InOutBack:
      return InOutBack(t);
    default:
      return Linear(t);
  }
}

float Easing::Linear(float t) { return t; }

float Easing::InQuad(float t) { return t * t; }

float Easing::OutQuad(float t) { return 1.0f - (1.0f - t) * (1.0f - t); }

float Easing::InOutQuad(float t) {
  return t < 0.5f ? 2.0f * t * t
                  : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float Easing::InCubic(float t) { return t * t * t; }

float Easing::OutCubic(float t) { return 1.0f - std::pow(1.0f - t, 3.0f); }

float Easing::InOutCubic(float t) {
  return t < 0.5f ? 4.0f * t * t * t
                  : 1.0f - std::pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

float Easing::InSine(float t) { return 1.0f - std::cos((t * kPi) / 2.0f); }

float Easing::OutSine(float t) { return std::sin((t * kPi) / 2.0f); }

float Easing::InOutSine(float t) { return -(std::cos(kPi * t) - 1.0f) / 2.0f; }

float Easing::InBounce(float t) { return 1.0f - OutBounce(1.0f - t); }

float Easing::OutBounce(float t) {
  const float n1 = 7.5625f;
  const float d1 = 2.75f;

  if (t < 1.0f / d1) {
    return n1 * t * t;
  } else if (t < 2.0f / d1) {
    t -= 1.5f / d1;
    return n1 * t * t + 0.75f;
  } else if (t < 2.5f / d1) {
    t -= 2.25f / d1;
    return n1 * t * t + 0.9375f;
  } else {
    t -= 2.625f / d1;
    return n1 * t * t + 0.984375f;
  }
}

float Easing::InOutBounce(float t) {
  return t < 0.5f ? (1.0f - OutBounce(1.0f - 2.0f * t)) / 2.0f
                  : (1.0f + OutBounce(2.0f * t - 1.0f)) / 2.0f;
}

float Easing::InElastic(float t) {
  const float c4 = (2.0f * kPi) / 3.0f;
  return t == 0.0f   ? 0.0f
         : t == 1.0f ? 1.0f
                     : -std::pow(2.0f, 10.0f * t - 10.0f) *
                           std::sin((t * 10.0f - 10.75f) * c4);
}

float Easing::OutElastic(float t) {
  const float c4 = (2.0f * kPi) / 3.0f;
  return t == 0.0f ? 0.0f
         : t == 1.0f
             ? 1.0f
             : std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) +
                   1.0f;
}

float Easing::InOutElastic(float t) {
  const float c5 = (2.0f * kPi) / 4.5f;
  return t == 0.0f   ? 0.0f
         : t == 1.0f ? 1.0f
         : t < 0.5f  ? -(std::pow(2.0f, 20.0f * t - 10.0f) *
                        std::sin((20.0f * t - 11.125f) * c5)) /
                          2.0f
                    : (std::pow(2.0f, -20.0f * t + 10.0f) *
                       std::sin((20.0f * t - 11.125f) * c5)) /
                              2.0f +
                          1.0f;
}

float Easing::InBack(float t) {
  const float c1 = 1.70158f;
  const float c3 = c1 + 1.0f;
  return c3 * t * t * t - c1 * t * t;
}

float Easing::OutBack(float t) {
  const float c1 = 1.70158f;
  const float c3 = c1 + 1.0f;
  return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

float Easing::InOutBack(float t) {
  const float c1 = 1.70158f;
  const float c2 = c1 * 1.525f;

  return t < 0.5f
             ? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
             : (std::pow(2.0f * t - 2.0f, 2.0f) *
                    ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) +
                2.0f) /
                   2.0f;
}

}  // namespace engine::util
