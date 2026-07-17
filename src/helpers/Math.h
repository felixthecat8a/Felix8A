#ifndef FELIX8A_MATH_H
#define FELIX8A_MATH_H

#include <Arduino.h>

namespace Felix8A {

  class Math {
    public:
      // Clamping: Keeps a value within a range.
      template<typename T>
      static inline T clamp(T value, T minVal, T maxVal) {
        if (value < minVal) return minVal;
        if (value > maxVal) return maxVal;
        return value;
      }

      // Linear Interpolation: Finds value between a and b based on t in [0, 1]
      template<typename T, typename U>
      static inline T lerp(T a, T b, U t) {
        return (T)(a + (b - a) * t);
      }

      // Inverse linear Interpolation: returns t such that value = lerp(a, b, t)
      template<typename T>
      static inline float inverseLerp(T a, T b, T value) {
        if (a == b) return 0.0f; // avoid division by zero
        return (float)(value - a) / (float)(b - a);
      }

      // Maps value from one range to another with clamping
      template<typename T, typename U = float>
      static inline U mapClamped(T value, T inMin, T inMax, U outMin, U outMax) {
        if (inMin == inMax) return outMin; // safety
        // U t = (U)(value - inMin) / (U)(inMax - inMin);
        U t = (U)inverseLerp(inMin, inMax, value);
        t = clamp(t, (U)0, (U)1);
        return lerp(outMin, outMax, t);
      }

      // Wraps value into [min, max)
      template<typename T>
      static inline T wrap(T value, T minVal, T maxVal) {
        T range = maxVal - minVal;
        if (range == 0) return minVal;

        // value = (value - minVal) % range;
        // if (value < 0) value += range;
        // return value + minVal;

        while (value < minVal) value += range;
        while (value >= maxVal) value -= range;

        return value;
      }

      // Wraps value into [min, max] (inclusive)
      template<typename T>
      static inline T wrapInclusive(T value, T minVal, T maxVal) {
        return wrap(value, minVal, maxVal + 1);
      }
  };

} // namespace Felix8A

#endif // FELIX8A_MATH_H