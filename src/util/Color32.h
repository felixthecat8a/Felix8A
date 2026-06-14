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

      // Shades of White
      static constexpr uint32_t BLACK           = 0x000000;
      static constexpr uint32_t DIM_WHITE       = 0x999999;
      static constexpr uint32_t WHITE           = 0xFFFFFF;
      static constexpr uint32_t WARM_WHITE      = 0xFFC0C0;
      static constexpr uint32_t COOL_WHITE      = 0xC0C0FF;
      static constexpr uint32_t HONEYDEW        = 0xC0FFC0;
      static constexpr uint32_t SNOW            = 0xFFC0D2;
      static constexpr uint32_t IVORY           = 0xFFFFC0;

      // Color Spectrum
      static constexpr uint32_t RED             = 0xFF0000;
      static constexpr uint32_t VERMILION       = 0xFF4000;
      static constexpr uint32_t ORANGE          = 0xFF8000;
      static constexpr uint32_t AMBER           = 0xFFBF00;
      static constexpr uint32_t YELLOW          = 0xFFFF00;
      static constexpr uint32_t LIME            = 0xBFFF00;
      static constexpr uint32_t CHARTREUSE      = 0x80FF00;
      static constexpr uint32_t HARLEQUIN       = 0x40FF00;
      static constexpr uint32_t GREEN           = 0x00FF00;
      static constexpr uint32_t MINT            = 0x00FF40;
      static constexpr uint32_t SPRING_GREEN    = 0x00FF80;
      static constexpr uint32_t TURQUOISE       = 0x00FFBF;
      static constexpr uint32_t CYAN            = 0x00FFFF;
      static constexpr uint32_t SKY_BLUE        = 0x00BFFF;
      static constexpr uint32_t AZURE           = 0x0080FF;
      static constexpr uint32_t SAPPHIRE        = 0x0040FF;
      static constexpr uint32_t BLUE            = 0x0000FF;
      static constexpr uint32_t INDIGO          = 0x4000FF;
      static constexpr uint32_t VIOLET          = 0x8000FF;
      static constexpr uint32_t E_PURPLE        = 0xBF00FF;
      static constexpr uint32_t MAGENTA         = 0xFF00FF;
      static constexpr uint32_t HOT_PINK        = 0xFF00BF;
      static constexpr uint32_t ROSE            = 0xFF0080;
      static constexpr uint32_t CRIMSON         = 0xFF0040;

      // Arduino Teal
      static constexpr uint32_t ARDUINO_TEAL    = 0x00878F;

      // Other Colors
      static constexpr uint32_t CERULEAN        = 0x007BA7;
      static constexpr uint32_t SEA_GREEN       = 0x2E8B57;
      static constexpr uint32_t AQUAMARINE      = 0x7FFFD4;
      static constexpr uint32_t LILAC           = 0xC8A2C8;
      static constexpr uint32_t LAVENDER        = 0xE6E6FA;

      // Darker Colors
      static constexpr uint32_t MAROON          = 0x800000;
      static constexpr uint32_t OLIVE           = 0x808000;
      static constexpr uint32_t AO              = 0x008000;
      static constexpr uint32_t TEAL            = 0x008080;
      static constexpr uint32_t NAVY            = 0x000080;
      static constexpr uint32_t PURPLE          = 0x800080;
      static constexpr uint32_t GRAY            = 0x808080;

      static constexpr uint32_t GREY = GRAY; // Alternative spelling

      static constexpr uint32_t OFF = BLACK; // Alias for off state

      // Hue, Saturation, Value (HSV) to RGB conversion
      inline uint32_t hsv(uint16_t h, uint8_t s, uint8_t v) {
        if (s == 0) {
          return hex(v, v, v); // Grayscale
        }

        h %= 360;

        uint8_t region = h / 60;
        uint8_t remainder = h % 60;
        uint8_t f = (remainder * 255) / 60;

        uint8_t p = (v * (255 - s)) / 255;
        uint8_t q = (v * (255 - ((uint16_t)s * f) / 255)) / 255;
        uint8_t t = (v * (255 - ((uint16_t)s * (255 - f)) / 255)) / 255;

        switch (region) {
          case 0: return hex(v, t, p);
          case 1: return hex(q, v, p);
          case 2: return hex(p, v, t);
          case 3: return hex(p, q, v);
          case 4: return hex(t, p, v);
          default: return hex(v, p, q);
        }
      }

      inline uint32_t hsv(uint16_t h) {
        return hsv(h, 255, 255);
      }
  };

} // namespace Felix8A

#endif // COLOR32_H
