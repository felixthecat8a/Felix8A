#ifndef FELIX_LED_COLOR_RGB_H
#define FELIX_LED_COLOR_RGB_H

#include <Arduino.h>

namespace Felix8A {
  // if i want to use this for something else, would this work? how about cmyk?
  struct ColorRGB {
    uint8_t r, g, b;

    constexpr ColorRGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}

    constexpr uint32_t hex() const { return (uint32_t(r) << 16) | (uint32_t(g) << 8) | b; }

    static constexpr ColorRGB fromHex(uint32_t hex) {
      return ColorRGB((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
    }

    static ColorRGB fromHSV(int hue, float sat, float val) {
      float h      = hue / 60.0f;
      int   sector = (int)h;
      float f      = h - sector;

      float p = val * (1.0f - sat);
      float q = val * (1.0f - sat * f);
      float t = val * (1.0f - sat * (1.0f - f));

      float r, g, b;

      // clang-format off
      switch (sector) {
        case 0: r = val; g = t; b = p; break;
        case 1: r = q; g = val; b = p; break;
        case 2: r = p; g = val; b = t; break;
        case 3: r = p; g = q; b = val; break;
        case 4: r = t; g = p; b = val; break;
        default: r = val; g = p; b = q; break;
      }
      // clang-format on

      uint8_t red   = constrain(roundf(r * 255), 0, 255);
      uint8_t green = constrain(roundf(g * 255), 0, 255);
      uint8_t blue  = constrain(roundf(b * 255), 0, 255);

      return ColorRGB(red, green, blue);
    }

    static ColorRGB fromCMYK(float cyan, float magenta, float yellow, float key) {
      cyan    = constrain(cyan, 0.0f, 1.0f);
      magenta = constrain(magenta, 0.0f, 1.0f);
      yellow  = constrain(yellow, 0.0f, 1.0f);
      key     = constrain(key, 0.0f, 1.0f);

      float invK = 1.0f - key;

      uint8_t red   = constrain(roundf(255 * (1.0f - cyan) * invK), 0, 255);
      uint8_t green = constrain(roundf(255 * (1.0f - magenta) * invK), 0, 255);
      uint8_t blue  = constrain(roundf(255 * (1.0f - yellow) * invK), 0, 255);

      return ColorRGB(red, green, blue);
    }
  };

  namespace RGB_Color {
    constexpr ColorRGB BLACK   = {0, 0, 0};
    constexpr ColorRGB WHITE   = {255, 255, 255};
    constexpr ColorRGB RED     = {255, 0, 0};
    constexpr ColorRGB ORANGE  = {255, 128, 0};
    constexpr ColorRGB YELLOW  = {255, 255, 0};
    constexpr ColorRGB LIME    = {128, 255, 0};
    constexpr ColorRGB GREEN   = {0, 255, 0};
    constexpr ColorRGB SPRING  = {0, 255, 128};
    constexpr ColorRGB CYAN    = {0, 255, 255};
    constexpr ColorRGB AZURE   = {0, 128, 255};
    constexpr ColorRGB BLUE    = {0, 0, 255};
    constexpr ColorRGB VIOLET  = {128, 0, 255};
    constexpr ColorRGB MAGENTA = {255, 0, 255};
    constexpr ColorRGB ROSE    = {255, 0, 128};
  } // namespace RGB_Color
} // namespace Felix8A

#endif // FELIX_LED_COLOR_RGB_H
