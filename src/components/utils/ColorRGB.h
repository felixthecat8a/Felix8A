#ifndef FELIX_LED_COLOR_RGB_H
#define FELIX_LED_COLOR_RGB_H

#include <Arduino.h>

namespace Felix8A
{
  struct RGB_Color
  {
    uint8_t r, g, b;

    constexpr uint32_t hex() const { return (uint32_t(r) << 16) | (uint32_t(g) << 8) | b; }
  };

  namespace ColorRGB
  {
    constexpr RGB_Color BLACK = {0, 0, 0};
    constexpr RGB_Color WHITE = {255, 255, 255};
    constexpr RGB_Color RED = {255, 0, 0};
    constexpr RGB_Color ORANGE = {255, 128, 0};
    constexpr RGB_Color YELLOW = {255, 255, 0};
    constexpr RGB_Color LIME = {128, 255, 0};
    constexpr RGB_Color GREEN = {0, 255, 0};
    constexpr RGB_Color SPRING = {0, 255, 128};
    constexpr RGB_Color CYAN = {0, 255, 255};
    constexpr RGB_Color AZURE = {0, 128, 255};
    constexpr RGB_Color BLUE = {0, 0, 255};
    constexpr RGB_Color VIOLET = {128, 0, 255};
    constexpr RGB_Color MAGENTA = {255, 0, 255};
    constexpr RGB_Color ROSE = {255, 0, 128};
  } // namespace ColorRGB
} // namespace Felix8A

#endif // FELIX_LED_COLOR_RGB_H
