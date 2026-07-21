#ifndef FELIX_LED_H
#define FELIX_LED_H

#include <Arduino.h>
#include "utils/DigitalOutput.h"

namespace Felix8A
{

  class LED : public DigitalOutput
  {
  public:
    using DigitalOutput::DigitalOutput;

    void on() { write(true); }
    void off() { write(false); }
    void toggle() { write(!state()); }
    bool isOn() const { return state(); }
    uint8_t getPin() const { return pin(); }
  };

  // class LED
  // {
  // public:
  //   explicit LED(uint8_t pin, bool activeLow = false) : _out(pin, activeLow) {}

  //   void begin() { _out.begin(); }

  //   void on() { _out.write(true); }
  //   void off() { _out.write(false); }
  //   void toggle() { _out.write(!_out.state()); }
  //   bool isOn() const { return _out.state(); }

  //   void setPin(uint8_t pin) { _out.setPin(pin); }
  //   uint8_t getPin() const { return _out.pin(); }

  //   void setActiveLow(bool activeLow) { _out.setActiveLow(activeLow); }
  //   bool isActiveLow() const { return _out.isActiveLow(); }

  // private:
  //   DigitalOutput _out;
  // };

} // namespace Felix8A

#endif // FELIX_LED_H
