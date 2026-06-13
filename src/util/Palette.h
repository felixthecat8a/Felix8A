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

      // indexed gradient stepping
      // uint32_t lerp(uint8_t t) const {
      //   if (_size == 0) return 0;
      //   return _colors[(uint16_t(t) * (_size - 1)) / 255];
      // }

      constexpr uint32_t lerp(uint8_t t) const {
        return (_size == 0)
          ? 0
          : _colors[(uint16_t(t) * (_size - 1)) / 255];
      }

      constexpr const uint32_t* data() const { return _colors; }

    private:
      const uint32_t* _colors;
      uint8_t _size;
  };

} // namespace Felix8A

#endif // PALETTE_H
