#include "FelixButton.h"

namespace Felix8A {

  Button::Button(uint8_t pin, bool activeLow, uint16_t debounceTime)
      : _input(pin, activeLow), _debounceTime(debounceTime) {}

  /* Lifecycle */
  void Button::begin() {
    _input.begin(true);
    _stableState = _input.read();
    _lastReading = _stableState;

    _state      = State::Idle;
    _event      = Event::None;
    _clickCount = 0;
    _holdFired  = false;
  }

  void Button::update() {
    uint32_t now     = millis();
    bool     reading = _input.read();

    if (reading != _lastReading) {
      _lastDebounceTime = now;
      _lastReading      = reading;
    }

    if ((now - _lastDebounceTime) < _debounceTime) { return; }

    if (reading != _stableState) {
      _stableState = reading;
      handleStableChange(now);
    }

    handleHold(now);
    handleClickTimeout(now);
  }

  /* Private Methods */

  void Button::handleStableChange(uint32_t now) {
    if (_stableState) {
      // PRESS
      _pressedTime = now;
      _holdFired   = false;

      if (_state == State::WaitingMulti && (now - _lastReleaseTime) <= _multiClickTime) {
        _clickCount++;
      } else {
        _clickCount = 1;
      }

      _event = Event::Press;
      _state = State::Pressed;
    } else {
      // RELEASE
      _event = Event::Release;

      if (_state == State::Held) {
        _resetClicks();
        _state = State::Idle;
        return;
      }

      _lastReleaseTime = now;
      _state           = State::WaitingMulti;
    }
  }

  void Button::handleHold(uint32_t now) {
    if (_state == State::Pressed && !_holdFired && (now - _pressedTime) >= _holdTime) {
      _event     = Event::Hold;
      _holdFired = true;
      _resetClicks();
      _state = State::Held;
    }
  }

  void Button::handleClickTimeout(uint32_t now) {
    if (_state == State::WaitingMulti && (now - _lastReleaseTime) > _multiClickTime) {

      if (_clickCount == 1) _event = Event::Click;
      else if (_clickCount == 2)
        _event = Event::DoubleClick;
      else if (_clickCount >= 3)
        _event = Event::TripleClick;

      _resetClicks();
      _state = State::Idle;
    }
  }

} // namespace Felix8A
