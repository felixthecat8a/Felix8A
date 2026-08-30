#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H

#include <Arduino.h>

namespace Felix8A {

  class AnalogInput {
  public:
    explicit AnalogInput(uint8_t pin) : _pin(pin) {}

    void begin() { pinMode(_pin, INPUT); }

    int read() const { return analogRead(_pin); }

    uint8_t pin() const { return _pin; }

    void setPin(uint8_t pin) { _pin = pin; }

  protected:
    uint8_t _pin;
  };

} // namespace Felix8A

#endif // ANALOG_INPUT_H
