#ifndef FELIX32_H
#define FELIX32_H

#include <Arduino.h>

#include "color/Color32.h"
#include "color/Palette.h"

#define FELIX32_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

namespace Felix8A {
  using Color = Color32;

  namespace Sets {

    static const uint32_t ChristmasTree5[] = {
        Color::RED,
        Color::ORANGE,
        Color::GREEN,
        Color::BLUE,
        Color::GRAY,
    };

    static const uint32_t Independence3[] = {
        Color::RED,
        Color::WHITE,
        Color::BLUE,
    };

    static const uint32_t Sunset9[] = {
        Color::MAROON,
        Color::RED,
        Color::VERMILION,
        Color::OLIVE,
        Color::ORANGE,
        Color::AMBER,
        Color::YELLOW,
        Color::MOCHA,
        Color::CORAL,
    };

    static const uint32_t Forest9[] = {
        Color::LIME,
        Color::CHARTREUSE,
        Color::HARLEQUIN,
        Color::EMERALD,
        Color::GREEN,
        Color::MINT,
        Color::SPRING_GREEN,
        Color::TURQUOISE,
        Color::HONEYDEW,
    };

    static const uint32_t Ocean9[] = {
        Color::TEAL,
        Color::CYAN,
        Color::SKY_BLUE,
        Color::AZURE,
        Color::SAPPHIRE,
        Color::NAVY,
        Color::BLUE,
        Color::LAVENDER,
        Color::CELESTE,
    };

    static const uint32_t Blush9[] = {
        Color::INDIGO,
        Color::VIOLET,
        Color::ELECTRIC_PURPLE,
        Color::PURPLE,
        Color::MAGENTA,
        Color::HOT_PINK,
        Color::ROSE,
        Color::CRIMSON,
        Color::LILAC,
    };

    static const uint32_t Spectrum36[] = {
        Color::CORAL,     Color::MAROON,       Color::RED,
        Color::VERMILION, Color::ORANGE,       Color::AMBER,
        Color::MOCHA,     Color::OLIVE,        Color::YELLOW,
        Color::LIME,      Color::CHARTREUSE,   Color::HARLEQUIN,
        Color::HONEYDEW,  Color::EMERALD,      Color::GREEN,
        Color::MINT,      Color::SPRING_GREEN, Color::TURQUOISE,
        Color::CELESTE,   Color::TEAL,         Color::CYAN,
        Color::SKY_BLUE,  Color::AZURE,        Color::SAPPHIRE,
        Color::LAVENDER,  Color::NAVY,         Color::BLUE,
        Color::INDIGO,    Color::VIOLET,       Color::ELECTRIC_PURPLE,
        Color::LILAC,     Color::PURPLE,       Color::MAGENTA,
        Color::HOT_PINK,  Color::ROSE,         Color::CRIMSON,
    };

    static const uint32_t Spectrum24[] = {
        Color::RED,     Color::VERMILION, Color::ORANGE,       Color::AMBER,
        Color::YELLOW,  Color::LIME,      Color::CHARTREUSE,   Color::HARLEQUIN,
        Color::GREEN,   Color::MINT,      Color::SPRING_GREEN, Color::TURQUOISE,
        Color::CYAN,    Color::SKY_BLUE,  Color::AZURE,        Color::SAPPHIRE,
        Color::BLUE,    Color::INDIGO,    Color::VIOLET,       Color::ELECTRIC_PURPLE,
        Color::MAGENTA, Color::HOT_PINK,  Color::ROSE,         Color::CRIMSON,
    };

    static const uint32_t Spectrum12[] = {
        Color::RED,
        Color::ORANGE,
        Color::YELLOW,
        Color::CHARTREUSE,
        Color::GREEN,
        Color::SPRING_GREEN,
        Color::CYAN,
        Color::AZURE,
        Color::BLUE,
        Color::VIOLET,
        Color::MAGENTA,
        Color::ROSE,
    };

    static const uint32_t Spectrum6[] =
        {Color::RED, Color::YELLOW, Color::GREEN, Color::CYAN, Color::BLUE, Color::MAGENTA};

    static const uint32_t Spectrum3[] = {Color::RED, Color::GREEN, Color::BLUE};

  } // namespace Sets

  static const Palette ChristmasTree(
      Sets::ChristmasTree5, FELIX32_ARRAY_SIZE(Sets::ChristmasTree5)
  );

  static const Palette Freedom(Sets::Independence3, FELIX32_ARRAY_SIZE(Sets::Independence3));

  static const Palette Sunset(Sets::Sunset9, FELIX32_ARRAY_SIZE(Sets::Sunset9));
  static const Palette Forest(Sets::Forest9, FELIX32_ARRAY_SIZE(Sets::Forest9));
  static const Palette Ocean(Sets::Ocean9, FELIX32_ARRAY_SIZE(Sets::Ocean9));
  static const Palette Blush(Sets::Blush9, FELIX32_ARRAY_SIZE(Sets::Blush9));

  static const Palette Palette36(Sets::Spectrum36, FELIX32_ARRAY_SIZE(Sets::Spectrum36));
  static const Palette Palette24(Sets::Spectrum24, FELIX32_ARRAY_SIZE(Sets::Spectrum24));
  static const Palette Palette12(Sets::Spectrum12, FELIX32_ARRAY_SIZE(Sets::Spectrum12));
  static const Palette Palette6(Sets::Spectrum6, FELIX32_ARRAY_SIZE(Sets::Spectrum6));
  static const Palette Palette3(Sets::Spectrum3, FELIX32_ARRAY_SIZE(Sets::Spectrum3));

  static inline uint32_t ColorMap(float value, float min, float max, uint32_t start, uint32_t end) {
    if (min >= max) { return start; }

    value = constrain(value, min, max);

    uint8_t t = ((value - min) * 255.0f) / (max - min);
    return Color::blend(start, end, t);
  }

  static inline uint32_t ColorMap(
      float value, float min, float center, float max, uint32_t cLow, uint32_t cMid, uint32_t cHigh
  ) {
    if (min >= center || center >= max) { return cLow; }

    value = constrain(value, min, max);

    if (value <= center) {
      uint8_t t = ((value - min) * 255.0f) / (center - min);
      return Color::blend(cLow, cMid, t);
    }

    uint8_t t = ((value - center) * 255.0f) / (max - center);
    return Color::blend(cMid, cHigh, t);
  }

  class ColorMap2 {
  public:
    static inline uint32_t set(float value, float min, float max, uint32_t start, uint32_t end) {
      if (min >= max) { return start; }

      value = constrain(value, min, max);

      uint8_t t = ((value - min) * 255.0f) / (max - min);
      return blend(start, end, t);
    }

    static inline uint32_t set(
        float    value,
        float    min,
        float    center,
        float    max,
        uint32_t cLow,
        uint32_t cMid,
        uint32_t cHigh
    ) {
      if (min >= center || center >= max) { return cLow; }

      value = constrain(value, min, max);

      if (value <= center) {
        uint8_t t = ((value - min) * 255.0f) / (center - min);
        return blend(cLow, cMid, t);
      }

      uint8_t t = ((value - center) * 255.0f) / (max - center);
      return blend(cMid, cHigh, t);
    }

  private:
    static constexpr uint32_t hex(uint8_t r, uint8_t g, uint8_t b) {
      return (uint32_t(r) << 16) | (uint32_t(g) << 8) | b;
    }

    static constexpr uint8_t red(uint32_t c) { return (c >> 16) & 0xFF; }
    static constexpr uint8_t green(uint32_t c) { return (c >> 8) & 0xFF; }
    static constexpr uint8_t blue(uint32_t c) { return c & 0xFF; }

    static inline uint32_t blend(uint32_t a, uint32_t b, uint8_t t) {
      auto lerp = [t](uint8_t x, uint8_t y) -> uint8_t {
        return x + ((int32_t(y) - int32_t(x)) * t) / 255;
      };

      uint8_t redBlend   = lerp(red(a), red(b));
      uint8_t greenBlend = lerp(green(a), green(b));
      uint8_t blueBlend  = lerp(blue(a), blue(b));

      return hex(redBlend, greenBlend, blueBlend);
    }
  };

} // namespace Felix8A

#endif // FELIX32_H
