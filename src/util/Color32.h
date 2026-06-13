#ifndef COLOR32_H
#define COLOR32_H

#include <Arduino.h>

namespace Felix8A {

  class Color32 {
    public:
      // Core
      static constexpr uint32_t hex(uint8_t r, uint8_t g, uint8_t b) {
        return (uint32_t(r) << 16) | (uint32_t(g) << 8) | b;
      }

      static constexpr uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
        return hex(r, g, b);
      }

      static constexpr uint32_t fromHex(uint32_t c) {
        return c & 0xFFFFFF;
      }

      // Channel extraction
      static constexpr uint8_t red(uint32_t c)   { return (c >> 16) & 0xFF; }
      static constexpr uint8_t green(uint32_t c) { return (c >> 8) & 0xFF; }
      static constexpr uint8_t blue(uint32_t c)  { return c & 0xFF; }

      // Brightness scaling
      static constexpr uint32_t scale(uint32_t c, uint8_t s) {
        return hex(
          (uint16_t(red(c))   * (s + 1)) >> 8,
          (uint16_t(green(c)) * (s + 1)) >> 8,
          (uint16_t(blue(c))  * (s + 1)) >> 8
        );
      }

      // Color Blending
      static inline uint32_t blend(uint32_t a, uint32_t b, uint8_t t) {
        auto lerp = [t](uint8_t x, uint8_t y) -> uint8_t {
          return x + ((int16_t(y) - int16_t(x)) * t) / 255;
        };

        return hex(
          lerp(red(a),   red(b)),
          lerp(green(a), green(b)),
          lerp(blue(a),  blue(b))
        );
      }

      static constexpr uint32_t BLACK        = 0x000000;
      static constexpr uint32_t DIM_WHITE    = 0x999999;
      static constexpr uint32_t WHITE        = 0xFFFFFF;
      static constexpr uint32_t COOL_WHITE   = 0xE6F2FF;
      static constexpr uint32_t WARM_WHITE   = 0xFFE6C7;

      static constexpr uint32_t RED          = 0xFF0000;
      static constexpr uint32_t VERMILION    = 0xFF4000;
      static constexpr uint32_t ORANGE       = 0xFF8000;
      static constexpr uint32_t AMBER        = 0xFFBF00;
      static constexpr uint32_t YELLOW       = 0xFFFF00;
      static constexpr uint32_t LIME         = 0xBFFF00;
      static constexpr uint32_t CHARTREUSE   = 0x80FF00;
      static constexpr uint32_t HARLEQUIN    = 0x40FF00;
      static constexpr uint32_t GREEN        = 0x00FF00;
      static constexpr uint32_t MINT         = 0x00FF40;
      static constexpr uint32_t SPRING_GREEN = 0x00FF80;
      static constexpr uint32_t TURQUOISE    = 0x00FFBF;
      static constexpr uint32_t CYAN         = 0x00FFFF;
      static constexpr uint32_t SKY_BLUE     = 0x00BFFF;
      static constexpr uint32_t AZURE        = 0x0080FF;
      static constexpr uint32_t SAPPHIRE     = 0x0040FF;
      static constexpr uint32_t BLUE         = 0x0000FF;
      static constexpr uint32_t INDIGO       = 0x4000FF;
      static constexpr uint32_t VIOLET       = 0x8000FF;
      static constexpr uint32_t PURPLE       = 0xBF00FF;
      static constexpr uint32_t MAGENTA      = 0xFF00FF;
      static constexpr uint32_t HOT_PINK     = 0xFF00BF;
      static constexpr uint32_t ROSE         = 0xFF0080;
      static constexpr uint32_t CRIMSON      = 0xFF0040;

      static constexpr uint32_t OFF = BLACK;
  };

} // namespace Felix8A

#endif // COLOR32_H
