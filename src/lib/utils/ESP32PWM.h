#ifndef ESP32PWM_H
#define ESP32PWM_H

#include <Arduino.h>

#ifdef ESP32

  #ifndef ESP32_PWM_FREQ
    #define ESP32_PWM_FREQ 5000
  #endif

  #ifndef ESP32_PWM_RES
    #define ESP32_PWM_RES 8
  #endif

namespace Felix8A {
  namespace ESP32PWM {

    static bool channelUsed[16] = {false};

    int8_t allocateChannel() {
      for (int i = 0; i < 16; i++) {
        if (!channelUsed[i]) {
          channelUsed[i] = true;
          return i;
        }
      }
      return -1;
    }

    void freeChannel(int8_t channel) {
      if (channel >= 0 && channel < 16) { channelUsed[channel] = false; }
    }

    void begin(uint8_t pin, int8_t& channel) {
  #if ESP_ARDUINO_VERSION_MAJOR >= 3
      ledcAttach(pin, ESP32_PWM_FREQ, ESP32_PWM_RES);
  #else
      if (channel < 0) {
        channel = allocateChannel();
        if (channel < 0) { return; }
      }
      ledcSetup(channel, ESP32_PWM_FREQ, ESP32_PWM_RES);
      ledcAttachPin(pin, channel);
  #endif
    }

    void write(uint8_t pin, int8_t channel, uint8_t value) {
  #if ESP_ARDUINO_VERSION_MAJOR >= 3
      ledcWrite(pin, value);
  #else
      ledcWrite(channel, value);
  #endif
    }

    void detach(uint8_t pin, int8_t channel) {
  #if ESP_ARDUINO_VERSION_MAJOR < 3
      if (channel >= 0) { ledcDetachPin(pin); }
  #endif
    }

  } // namespace ESP32PWM
} // namespace Felix8A

#endif // ESP32

#endif // ESP32PWM_H
