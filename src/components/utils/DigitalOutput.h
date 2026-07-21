#ifndef DIGITAL_OUTPUT_H
#define DIGITAL_OUTPUT_H

#include <Arduino.h>

namespace Felix8A
{

  class DigitalOutput
  {
  public:
    explicit DigitalOutput(uint8_t pin, bool activeLow = false)
        : _pin(pin), _activeLow(activeLow) {}

    void begin()
    {
      pinMode(_pin, OUTPUT);
      write(false); // default OFF
    }

    void write(bool on)
    {
      _state = on;
      bool level = _activeLow ? !on : on;
      digitalWrite(_pin, level ? HIGH : LOW);
    }

    bool state() const { return _state; }

    void setActiveLow(bool v) { _activeLow = v; }
    bool isActiveLow() const { return _activeLow; }

    uint8_t pin() const { return _pin; }
    void setPin(uint8_t pin) { _pin = pin; }

  protected:
    uint8_t _pin;
    bool _activeLow;
    bool _state = false;
  };

} // namespace Felix8A

#endif // DIGITAL_OUTPUT_H
