#ifndef FELIX_PWM_H
#define FELIX_PWM_H

#include <Arduino.h>

#include "utils/ColorRGB.h"
#include "utils/ESP32PWM.h"

#ifdef ARDUINO_ARCH_AVR
  #include <avr/pgmspace.h>
  #define READ_GAMMA(x) pgm_read_byte(&_gammaTable[x])
#else
  #define READ_GAMMA(x) _gammaTable[x]
#endif

#ifndef PWM_MAX
  #define PWM_MAX 255
#endif

namespace Felix8A {

  /* PWM LED */

  enum LED_t : uint8_t { BASIC_LED, ESP32_LED };

  class PWM {
  public:
    explicit PWM(uint8_t pin, bool activeLow = false, LED_t type = BASIC_LED, int8_t channel = -1);
    ~PWM();

    void    begin();
    void    setPWM(uint8_t level);
    void    setGlow(uint8_t glow);
    void    setBrightness(uint8_t brightness);
    uint8_t getBrightness() const { return _brightness; }
    void    on();
    void    off();
    void    toggle();
    bool    isOn() const { return _state; }
    void    setPin(uint8_t pin);
    uint8_t getPin() const { return _pin; }
    void    setActiveLow(bool activeLow);
    bool    isActiveLow() const { return _activeLow; }

  private:
    void _writeRaw(uint8_t value);

    uint8_t _pin;
    bool    _activeLow;
    LED_t   _type;
    int8_t  _channel;

    uint8_t _brightness = 0;
    bool    _state      = false;
  };

  /* RGB LED */

  class RGB {
  public:
    // clang-format off
    RGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool commonAnode = true,
      LED_t type = BASIC_LED, int8_t rCh = -1, int8_t gCh = -1, int8_t bCh = -1);
    // clang-format on
    void begin();
    // Color
    void setRGB(const ColorRGB& c);
    void setRGB(const uint8_t rgb[3]);
    void setRGB(uint8_t red, uint8_t green, uint8_t blue);
    void setRGB(uint32_t color);
    void setHex(uint32_t hex);
    // Brightness
    void setBrightness(uint8_t brightness);
    void setGlow(uint8_t glow) { setBrightness(glow); }
    // Color accessors
    uint8_t  getRed() const { return _color.r; }
    uint8_t  getGreen() const { return _color.g; }
    uint8_t  getBlue() const { return _color.b; }
    uint32_t getHex() const { return _color.hex(); }
    uint8_t  getBrightness() const { return _brightness; }
    String   getHexString() const;
    // Preset colors
    void off() { setRGB(RGB_Color::BLACK); }
    void setWhite() { setRGB(RGB_Color::WHITE); }
    void setRed() { setRGB(RGB_Color::RED); }
    void setOrange() { setRGB(RGB_Color::ORANGE); }
    void setYellow() { setRGB(RGB_Color::YELLOW); }
    void setLime() { setRGB(RGB_Color::LIME); }
    void setGreen() { setRGB(RGB_Color::GREEN); }
    void setSpring() { setRGB(RGB_Color::SPRING); }
    void setCyan() { setRGB(RGB_Color::CYAN); }
    void setAzure() { setRGB(RGB_Color::AZURE); }
    void setBlue() { setRGB(RGB_Color::BLUE); }
    void setViolet() { setRGB(RGB_Color::VIOLET); }
    void setMagenta() { setRGB(RGB_Color::MAGENTA); }
    void setRose() { setRGB(RGB_Color::ROSE); }
    // Color models
    void setHSV(int hue, float sat = 1.0f, float val = 1.0f);
    void setHue(int hue) { setHSV(hue, _sat, _val); }
    void setCMYK(float cyan, float magenta, float yellow, float key);
    // Gamma correction
    void setGammaCorrection(bool enabled);

  private:
    PWM      _rPWM, _gPWM, _bPWM;
    bool     _isCommonAnode;
    ColorRGB _color;

    uint8_t _brightness   = 255;
    bool    _gammaEnabled = false;

    int   _hue = 0;
    float _sat = 1.0f;
    float _val = 1.0f;

    inline uint8_t _applyBrightness(uint8_t c) const {
      return (uint16_t(c) * _brightness) / PWM_MAX;
    }

    inline uint8_t _applyGamma(uint8_t c) const { return _gammaEnabled ? READ_GAMMA(c) : c; }

    inline uint8_t _process(uint8_t c) const {
      c = _applyBrightness(c);
      c = _applyGamma(c);
      return c;
    }

    inline void _showRGB(uint8_t r, uint8_t g, uint8_t b) {
      _color = {r, g, b};
      _rPWM.setPWM(_process(r));
      _gPWM.setPWM(_process(g));
      _bPWM.setPWM(_process(b));
    }

    static const uint8_t _gammaTable[256] PROGMEM;
  };

} // namespace Felix8A

#endif // FELIX_PWM_H
