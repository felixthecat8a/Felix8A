#ifndef FELIX_LED_H
#define FELIX_LED_H

#include <Arduino.h>

#include "utils/DigitalOutput.h"
#include "utils/Time8A.h"

namespace Felix8A {

  class LED {
  public:
    explicit LED(uint8_t pin, bool activeLow = false) : _out(pin, activeLow) {}

    void begin() { _out.begin(); }

    // void update() {
    //   if (Time8A::once(_flashDuration, _flashStart)) { _out.write(false); }
    // }

    void on() {
      // Time8A::disarm(_flashStart);
      _out.write(true);
    }

    void off() {
      // Time8A::disarm(_flashStart);
      _out.write(false);
    }

    void toggle() {
      // Time8A::disarm(_flashStart);
      _out.toggle();
    }

    bool isOn() const { return _out.state(); }

    // void flash(unsigned long duration) {
    //   _flashDuration = duration;
    //   Time8A::arm(_flashStart);
    //   _out.write(true);
    // }

    // bool isFlashing() const { return Time8A::isArmed(_flashStart); }

    void    setPin(uint8_t pin) { _out.setPin(pin); }
    uint8_t getPin() const { return _out.pin(); }

    void setActiveLow(bool activeLow) { _out.setActiveLow(activeLow); }
    bool isActiveLow() const { return _out.isActiveLow(); }

  private:
    DigitalOutput _out;

    // unsigned long _flashStart = 0;
    // unsigned long _flashDuration = 0;
  };

  class Relay : public DigitalOutput {
  public:
    using DigitalOutput::DigitalOutput;

    void enable() { write(true); }
    void disable() { write(false); }
    void toggle() { toggle(); }
    bool isEnabled() const { return state(); }
    void set(bool enabled) { write(enabled); }
  };

  class Buzzer {
  public:
    explicit Buzzer(uint8_t pin, bool activeLow = false) : _out(pin, activeLow) {}

    void begin() { _out.begin(); }

    void update() {
      if (Time8A::once(_beepDuration, _beepStart)) { _out.write(false); }
    }

    void on() {
      Time8A::disarm(_beepStart);
      _out.write(true);
    }

    void off() {
      Time8A::disarm(_beepStart);
      _out.write(false);
    }

    void toggle() {
      Time8A::disarm(_beepStart);
      _out.toggle();
    }

    bool isOn() const { return _out.state(); }

    void beep(unsigned long duration) {
      _beepDuration = duration;
      Time8A::arm(_beepStart);
      _out.write(true);
    }

    bool isBeeping() const { return Time8A::isArmed(_beepStart); }

    void    setPin(uint8_t pin) { _out.setPin(pin); }
    uint8_t getPin() const { return _out.pin(); }

    void setActiveLow(bool activeLow) { _out.setActiveLow(activeLow); }
    bool isActiveLow() const { return _out.isActiveLow(); }

  private:
    DigitalOutput _out;

    unsigned long _beepStart    = 0;
    unsigned long _beepDuration = 0;
  };

} // namespace Felix8A

#endif // FELIX_LED_H
