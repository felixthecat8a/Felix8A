#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H

#include <Arduino.h>

namespace Felix8A {

  class DigitalInput {
  public:
    explicit DigitalInput(uint8_t pin, bool activeLow = true) : _pin(pin), _activeLow(activeLow) {}

    virtual ~DigitalInput() = default;

    virtual void begin(bool usePullup = true) {
      if (_activeLow && usePullup) {
        pinMode(_pin, INPUT_PULLUP);
      } else {
        pinMode(_pin, INPUT);
      }

      update();
    }

    bool read() const {
      bool raw = digitalRead(_pin);
      return _activeLow ? !raw : raw;
    }

    virtual void update() { _state = read(); }

    bool state() const { return _state; }

    void setActiveLow(bool v) {
      _activeLow = v;
      update();
    }

  protected:
    bool readRaw() const { return digitalRead(_pin); }

    uint8_t _pin;
    bool    _activeLow;
    bool    _state = false;
  };

} // namespace Felix8A

#endif // DIGITAL_INPUT_H
