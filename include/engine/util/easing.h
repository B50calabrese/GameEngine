/**
 * @file easing.h
 * @brief Easing functions for animations and tweens.
 */

#ifndef INCLUDE_ENGINE_UTIL_EASING_H_
#define INCLUDE_ENGINE_UTIL_EASING_H_

namespace engine::util {

/**
 * @brief Types of easing functions.
 */
enum class EaseType {
  Linear,
  InQuad,
  OutQuad,
  InOutQuad,
  InCubic,
  OutCubic,
  InOutCubic,
  InSine,
  OutSine,
  InOutSine,
  InBounce,
  OutBounce,
  InOutBounce,
  InElastic,
  OutElastic,
  InOutElastic,
  InBack,
  OutBack,
  InOutBack
};

/**
 * @brief Provides various easing functions to calculate non-linear interpolation.
 */
class Easing {
 public:
  /**
   * @brief Applies an easing function to a normalized time value.
   *
   * @param type The type of easing function to apply.
   * @param t The normalized time value (0.0 to 1.0).
   * @return The eased value.
   */
  static float Apply(EaseType type, float t);

  static float Linear(float t);

  static float InQuad(float t);
  static float OutQuad(float t);
  static float InOutQuad(float t);

  static float InCubic(float t);
  static float OutCubic(float t);
  static float InOutCubic(float t);

  static float InSine(float t);
  static float OutSine(float t);
  static float InOutSine(float t);

  static float InBounce(float t);
  static float OutBounce(float t);
  static float InOutBounce(float t);

  static float InElastic(float t);
  static float OutElastic(float t);
  static float InOutElastic(float t);

  static float InBack(float t);
  static float OutBack(float t);
  static float InOutBack(float t);
};

}  // namespace engine::util

#endif  // INCLUDE_ENGINE_UTIL_EASING_H_
