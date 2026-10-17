#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

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

  class Potentiometer : public AnalogInput {

  public:
    explicit Potentiometer(uint8_t pin, uint16_t maxAngle = 270)
        : AnalogInput(pin), _maxAngle(maxAngle) {}

    float voltage(float reference = 5.0f) const { return (read() / 1023.0f) * reference; }

    uint8_t percent() const { return static_cast<uint8_t>(map(read(), 0, 1023, 0, 100)); }

    uint16_t angle() const { return static_cast<uint16_t>(map(read(), 0, 1023, 0, _maxAngle)); }

    uint16_t maxAngle() const { return _maxAngle; }

    void setMaxAngle(uint16_t angle) { _maxAngle = angle; }

  private:
    uint16_t _maxAngle;
  };

} // namespace Felix8A

#endif // POTENTIOMETER_H
