#ifndef MATH_H
#define MATH_H

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

        while (value < minVal) value += range;
        while (value >= maxVal) value -= range;

        return value;
      }
      // template<typename T>
      // static inline T wrap(T value, T minVal, T maxVal) {
      //   T range = maxVal - minVal;
      //   if (range == 0) return minVal;
      //   value = (value - minVal) % range;
      //   if (value < 0) value += range;
      //   return value + minVal;
      // }

      // Wraps value into [min, max] (inclusive)
      template<typename T>
      static inline T wrapInclusive(T value, T minVal, T maxVal) {
        return wrap(value, minVal, maxVal + 1);
      }
  };

  class Temperature {
    public:
      // Celsius/Fahrenheit
      static float CtoF(float c) { return c * 9.0f / 5.0f + 32.0f; }
      static float FtoC(float f) { return (f - 32.0f) * 5.0f / 9.0f; }

      // Celsius/Kelvin
      static float CtoK(float c) { return c + 273.15f; }
      static float KtoC(float k) { return k - 273.15f; }

      // Fahrenheit/Kelvin
      static float FtoK(float f) { return CtoK(FtoC(f)); }
      static float KtoF(float k) { return CtoF(KtoC(k)); }
  };

  class TemperatureColorHue {
    public:
      static constexpr float GREEN_HUE = 120.0f;
      static constexpr float HUE_SHIFT_RANGE = 120.0f; // How far hue can shift to red or blue

      static float fromCelsius(float tempC, float greenPointC = 21.0f, float maxShiftC = 11.0f) {
        float diff = Math::clamp(tempC - greenPointC, -maxShiftC, maxShiftC);
        float hueShift = diff * (HUE_SHIFT_RANGE / maxShiftC);
        float hue = GREEN_HUE - hueShift;
        return Math::clamp(hue, 0.0f, 240.0f);
      }

      static float fromFahrenheit(float tempF, float greenPointF = 72.0f, float maxShiftF = 25.0f) {
        float tempC = Temperature::FtoC(tempF);
        return fromCelsius(tempC, Temperature::FtoC(greenPointF), Temperature::FtoC(maxShiftF));
      }
  };

} // namespace Felix8A

#endif // MATH_H
