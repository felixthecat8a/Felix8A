#ifndef PALETTE_H
#define PALETTE_H

#include <Arduino.h>

namespace Felix8A {

  class Palette {
    public:
      constexpr Palette(const uint32_t* colors, uint8_t size)
        : _colors(colors), _size(size) {}

      template <size_t N>
      constexpr Palette(const uint32_t (&colors)[N])
        : _colors(colors), _size(N) {}

      constexpr uint8_t size() const { return _size; }
      constexpr uint8_t count() const { return _size; }

      constexpr uint32_t operator[](uint8_t i) const {
        return (_size == 0) ? 0 : _colors[i % _size];
      }

      constexpr uint32_t reversed(uint8_t i) const {
        return (_size == 0) ? 0 : _colors[_size - 1 - (i % _size)];
      }

      constexpr const uint32_t* data() const { return _colors; }

      static constexpr uint8_t lerp8(uint8_t a, uint8_t b, uint8_t t) {
        return a + ((uint16_t)(b - a) * t) / 255;
      }

      static constexpr uint32_t blend(uint32_t c1, uint32_t c2, uint8_t t) {
        return (
          (uint32_t(lerp8((c1 >> 16) & 0xFF, (c2 >> 16) & 0xFF, t)) << 16) |
          (uint32_t(lerp8((c1 >> 8)  & 0xFF, (c2 >> 8)  & 0xFF, t)) << 8)  |
          (uint32_t(lerp8((c1 >> 0)  & 0xFF, (c2 >> 0)  & 0xFF, t)) << 0)
        );
      }

    private:
      const uint32_t* _colors;
      uint8_t _size;
  };

} // namespace Felix8A

#endif // PALETTE_H
