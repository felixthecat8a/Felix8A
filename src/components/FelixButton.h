#ifndef FELIX_BUTTON_H
#define FELIX_BUTTON_H

#include "utils/DigitalInput.h"
#include <Arduino.h>

namespace Felix8A {

  class Button {
  public:
    enum class Event : uint8_t { None, Press, Release, Click, DoubleClick, TripleClick, Hold };

    explicit Button(uint8_t pin, bool activeLow = true, uint16_t debounceTime = 50);

    /* Lifecycle */
    void begin();
    void update();

    /* Queries */
    bool isDown() const { return _stableState; }
    bool isHeld() const { return _state == State::Held; }

    /* Event Polling */
    Event poll() {
      Event e = _event;
      _event  = Event::None;
      return e;
    }

    /* Event Methods */
    bool wasPressed() { return _consume(Event::Press); }
    bool wasReleased() { return _consume(Event::Release); }
    bool wasClicked() { return _consume(Event::Click); }
    bool wasDoubleClicked() { return _consume(Event::DoubleClick); }
    bool wasTripleClicked() { return _consume(Event::TripleClick); }
    bool wasHeld() { return _consume(Event::Hold); }

    /* Configuration */
    void setHoldTime(uint16_t ms) { _holdTime = ms; }
    void setMultiClickTime(uint16_t ms) { _multiClickTime = ms; }

  private:
    enum class State : uint8_t { Idle, Pressed, Held, WaitingMulti };

    void handleStableChange(uint32_t now);
    void handleHold(uint32_t now);
    void handleClickTimeout(uint32_t now);

    void _resetClicks() { _clickCount = 0; }

    bool _consume(Event e) {
      if (_event == e) {
        _event = Event::None;
        return true;
      }
      return false;
    }

    /* Data */
    DigitalInput _input;
    uint16_t     _debounceTime;

    uint16_t _holdTime       = 1000;
    uint16_t _multiClickTime = 300;

    State _state = State::Idle;
    Event _event = Event::None;

    uint32_t _lastDebounceTime = 0;
    uint32_t _pressedTime      = 0;
    uint32_t _lastReleaseTime  = 0;

    bool _stableState = false;
    bool _lastReading = false;
    bool _holdFired   = false;

    uint8_t _clickCount = 0;
  };

} // namespace Felix8A

#endif // FELIX_BUTTON_H
