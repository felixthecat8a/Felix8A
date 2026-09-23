#ifndef FELIX8A_HELPERS_H
#define FELIX8A_HELPERS_H

#include <Arduino.h>

namespace Felix8A {

  /**
   * @brief Clamps a value to a specified range.
   *
   * @tparam T Value type.
   * @param value Value to clamp.
   * @param minVal Minimum allowed value.
   * @param maxVal Maximum allowed value.
   * @return The clamped value.
   */
  template <typename T> inline T clamp(T value, T minVal, T maxVal) {
    if (value < minVal) { return minVal; }

    if (value > maxVal) { return maxVal; }

    return value;
  }

  /**
   * @brief Performs linear interpolation between two values.
   *
   * @tparam T Output value type.
   * @tparam U Interpolation parameter type.
   * @param a Start value.
   * @param b End value.
   * @param t Interpolation parameter.
   * @return Interpolated value.
   *
   * @note t is not clamped. Values outside [0, 1] are allowed.
   */
  template <typename T, typename U> inline T lerp(T a, T b, U t) {
    return static_cast<T>(a + (b - a) * t);
  }

  /**
   * @brief Calculates the normalized position of a value between two values.
   *
   * @tparam T Value type.
   * @param a Start value.
   * @param b End value.
   * @param value Value to evaluate.
   * @return Normalized position as a float.
   *
   * @note The result is not clamped and may be outside [0, 1].
   *       Returns 0.0f if a and b are equal.
   */
  template <typename T> inline float inverseLerp(T a, T b, T value) {
    if (a == b) { return 0.0f; }

    return static_cast<float>(value - a) / static_cast<float>(b - a);
  }

  /**
   * @brief Maps a value from one range to another with clamping.
   *
   * @tparam T Input value type.
   * @tparam U Output value type.
   * @param value Input value.
   * @param inMin Input range minimum.
   * @param inMax Input range maximum.
   * @param outMin Output range minimum.
   * @param outMax Output range maximum.
   * @return Mapped and clamped value.
   *
   * @note If inMin and inMax are equal, outMin is returned.
   */
  template <typename T, typename U = float>
  inline U mapClamped(T value, T inMin, T inMax, U outMin, U outMax) {
    if (inMin == inMax) { return outMin; }

    U t = static_cast<U>(inverseLerp(inMin, inMax, value));
    t   = clamp(t, static_cast<U>(0), static_cast<U>(1));

    return lerp(outMin, outMax, t);
  }

  /**
   * @brief Wraps an integer value into the range [minVal, maxVal).
   *
   * @tparam T Integer value type.
   * @param value Value to wrap.
   * @param minVal Minimum value, inclusive.
   * @param maxVal Maximum value, exclusive.
   * @return Wrapped value.
   *
   * @note This function is intended for integer types.
   * @note If maxVal <= minVal, minVal is returned.
   */
  template <typename T> inline T wrap(T value, T minVal, T maxVal) {
    const T range = maxVal - minVal;

    if (range <= static_cast<T>(0)) { return minVal; }

    value = static_cast<T>((value - minVal) % range);

    if (value < static_cast<T>(0)) { value += range; }

    return value + minVal;
  }

  // /**
  //  * @brief Wraps an integer value into the range [minVal, maxVal].
  //  *
  //  * @tparam T Integer value type.
  //  * @param value Value to wrap.
  //  * @param minVal Minimum value, inclusive.
  //  * @param maxVal Maximum value, inclusive.
  //  * @return Wrapped value.
  //  */
  // template <typename T> static inline T wrapInclusive(T value, T minVal, T maxVal) {
  //   if (maxVal < minVal) return minVal;
  //   return wrap(value, minVal, maxVal + 1);
  // }

  /**
   * @brief Wraps an integer value into the range [minVal, maxVal].
   *
   * @tparam T Integer value type.
   * @param value Value to wrap.
   * @param minVal Minimum value, inclusive.
   * @param maxVal Maximum value, inclusive.
   * @return Wrapped value.
   *
   * @note This function is intended for integer types.
   * @note If maxVal < minVal, minVal is returned.
   */
  template <typename T> inline T wrapInclusive(T value, T minVal, T maxVal) {
    if (maxVal < minVal) { return minVal; }

    // Avoid maxVal + 1 overflowing T.
    if (value >= minVal && value <= maxVal) { return value; }

    const auto range = static_cast<long long>(maxVal) - static_cast<long long>(minVal) + 1LL;

    if (range <= 0) { return minVal; }

    long long wrapped = (static_cast<long long>(value) - static_cast<long long>(minVal)) % range;

    if (wrapped < 0) { wrapped += range; }

    return static_cast<T>(static_cast<long long>(minVal) + wrapped);
  }

} // namespace Felix8A

#endif // FELIX8A_HELPERS_H
