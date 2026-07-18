#include "FelixPWM.h"

namespace Felix8A {

  /* ESP32 Helpers */

  #ifdef ESP32
    static bool _ledcChannelUsed[16] = { false };

    static int8_t allocateLEDCChannel() {
      for (int i = 0; i < 16; i++) {
        if (!_ledcChannelUsed[i]) {
          _ledcChannelUsed[i] = true;
          return i;
        }
      }
      return -1; // none available
    }

    static void freeLEDCChannel(int8_t ch) {
      if (ch >= 0 && ch < 16) {
        _ledcChannelUsed[ch] = false;
      }
    }
  #endif

  #ifndef ESP32_PWM_FREQ
    #define ESP32_PWM_FREQ 5000 // 5 kHz frequency
  #endif

  #ifndef ESP32_PWM_RES
    #define ESP32_PWM_RES 8 // 8-bit resolution
  #endif

  /* PWM LED */

  PWM::PWM(uint8_t pin, bool activeLow, LED_t type, int8_t channel)
    : _pin(pin), _activeLow(activeLow), _type(type), _channel(channel) {}

  PWM::~PWM() {
    #ifdef ESP32
      #if ESP_ARDUINO_VERSION_MAJOR < 3
        if (_type == ESP32_LED && _channel >= 0) {
          freeLEDCChannel(_channel);
        }
      #endif
    #endif
  }

  void PWM::begin() {
    switch (_type) {
      case BASIC_LED:
        pinMode(_pin, OUTPUT);
        break;

      case ESP32_LED:
        #ifdef ESP32
          #if ESP_ARDUINO_VERSION_MAJOR >= 3
            ledcAttach(_pin, ESP32_PWM_FREQ, ESP32_PWM_RES);
          #else
            if (_channel < 0) {
              _channel = allocateLEDCChannel();
              if (_channel < 0) return;
            }
            ledcSetup(_channel, ESP32_PWM_FREQ, ESP32_PWM_RES);
            ledcAttachPin(_pin, _channel);
          #endif
        #endif
        break;
    }

    _writeRaw(_activeLow ? PWM_MAX : 0);
  }

  void PWM::setPWM(uint8_t level) {
    _brightness = level;
    uint8_t value = _activeLow ? (PWM_MAX - _brightness) : _brightness;
    _writeRaw(value);
    _state = (_brightness > 0);
  }

  void PWM::setGlow(uint8_t glow) {
    setPWM(glow);
  }

  void PWM::setBrightness(uint8_t level) {
    setPWM(level);
  }

  void PWM::on() {
    setPWM(PWM_MAX);
  }

  void PWM::off() {
    setPWM(0);
  }

  void PWM::toggle() {
    setPWM(_state ? 0 : PWM_MAX);
  }

  void PWM::setPin(uint8_t pin) {
    #ifdef ESP32
      #if ESP_ARDUINO_VERSION_MAJOR < 3
        if (_type == ESP32_LED && _channel >= 0) {
          ledcDetachPin(_pin);
        }
      #endif
    #endif

    _pin = pin;
    begin();
  }

  void PWM::setActiveLow(bool activeLow) {
    _activeLow = activeLow;
    setPWM(_brightness);
  }

  void PWM::_writeRaw(uint8_t value) {
    switch (_type) {
      case BASIC_LED:
        analogWrite(_pin, value);
        break;

      case ESP32_LED:
        #ifdef ESP32
          #if ESP_ARDUINO_VERSION_MAJOR >= 3
            ledcWrite(_pin, value);
          #else
            ledcWrite(_channel, value);
          #endif
        #endif
        break;
    }
  }

  /* RGB LED */

  RGB::RGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool commonAnode,
    LED_t type, int8_t rCh, int8_t gCh, int8_t bCh)
    : _rPWM(rPin, commonAnode, type, rCh),
      _gPWM(gPin, commonAnode, type, gCh),
      _bPWM(bPin, commonAnode, type, bCh) {}

  void RGB::begin() {
    _rPWM.begin();
    _gPWM.begin();
    _bPWM.begin();

    _showRGB(0, 0, 0);
  }

  void RGB::setRGB(const RGB_Color& c) {
    _showRGB(c.r, c.g, c.b);
  }

  void RGB::setRGB(uint8_t red, uint8_t green, uint8_t blue) {
    _showRGB(red, green, blue);
  }

  void RGB::setRGB(const uint8_t rgb[3]) {
    _showRGB(rgb[0], rgb[1], rgb[2]);
  }

  void RGB::setRGB(uint32_t hex) {
    _showRGB((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
  }

  void RGB::setBrightness(uint8_t level) {
    _brightness = constrain(level, 0, 255);
    _showRGB(_color.r, _color.g, _color.b);
  }

  void RGB::setGlow(uint8_t glow) {
    setBrightness(glow);
  }

  void RGB::setHex(uint32_t hex) {
    _showRGB((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
  }

  String RGB::getHexString() const {
    char buf[8];
    snprintf(buf, sizeof(buf), "#%06lX", getHex());
    return String(buf);
  }

  void RGB::setHSV(int hue, float sat, float val) {
    _hue = constrain(hue, 0, 359);
    _sat = constrain(sat, 0.0, 1.0f);
    _val = constrain(val, 0.0, 1.0f);
    const int SECTOR_SIZE = 60;
    int hueSector = _hue / SECTOR_SIZE;
    float hueFraction = (_hue / (float)SECTOR_SIZE) - hueSector;
    float minVal = _val * (1.0f - _sat);
    float intVal1 = _val * (1.0f - _sat * hueFraction);
    float intVal2 = _val * (1.0f - _sat * (1.0f - hueFraction));
    float red, green, blue;
    switch (hueSector % 6) {
      case 0: red = _val, green = intVal2, blue = minVal; break;
      case 1: red = intVal1, green = _val, blue = minVal; break;
      case 2: red = minVal, green = _val, blue = intVal2; break;
      case 3: red = minVal, green = intVal1, blue = _val; break;
      case 4: red = intVal2, green = minVal, blue = _val; break;
      case 5: red = _val, green = minVal, blue = intVal1; break;
      default: red = green = blue = 0; break;
    }
    _showRGB(roundf(red * PWM_MAX), roundf(green * PWM_MAX), roundf(blue * PWM_MAX));
  }

  void RGB::setCMYK(float cyan, float magenta, float yellow, float key) {
    cyan = constrain(cyan, 0.0f, 1.0f);
    magenta = constrain(magenta, 0.0f, 1.0f);
    yellow = constrain(yellow, 0.0f, 1.0f);
    key = constrain(key, 0.0f, 1.0f);
    uint8_t red = roundf(PWM_MAX * (1.0f - cyan) * (1.0f - key));
    uint8_t green = roundf(PWM_MAX * (1.0f - magenta) * (1.0f - key));
    uint8_t blue = roundf(PWM_MAX * (1.0f - yellow) * (1.0f - key));
    _showRGB(red, green, blue);
  }

  void RGB::setGammaCorrection(bool enabled) {
    _gammaEnabled = enabled;
    _showRGB(_color.r, _color.g, _color.b);
  }

  const uint8_t RGB::_gammaTable[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6,
    6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 13, 14, 14, 15, 16,
    17, 18, 19, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 34, 35, 36, 37, 38, 40, 41, 42, 44, 45, 46, 48, 49, 51, 52,
    54, 55, 57, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 76, 78, 80,
    82, 84, 86, 88, 90, 92, 94, 96, 98, 101, 103, 105, 107, 110, 112, 114,
    117, 119, 122, 124, 127, 129, 132, 134, 137, 140, 142, 145, 148, 151, 153, 156,
    159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 189, 192, 195, 198, 201, 205,
    208, 211, 214, 218, 221, 224, 228, 231, 235, 238, 242, 245, 249, 252, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
  };

} // namespace Felix8A
