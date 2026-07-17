/**
 * @file color.ino
 * @brief Example sketch for the Felix8A RGB LED controller.
 *
 * @details
 * This sketch demonstrates basic color setting using the Color32 utility class.
 * It cycles through a selection of predefined colors from the Felix8A::Color class,
 * changing the LED color every second.
 *
 * @section wiring Wiring
 * Select pins with ~ for pwm
 * * Red pin   → PIN_RED   (9)
 * * Green pin → PIN_GREEN (10)
 * * Blue pin  → PIN_BLUE  (11)
 *
 * LED type:
 * * Common Cathode:
 * * Connect common pin to GND
 * * PWM values: 0 = OFF, 255 = FULL brightness
 *
 * * Common Anode:
 * * Connect common pin to +5V
 * * PWM values are inverted: 0 = FULL brightness, 255 = OFF
 * * Software must invert values (255 - value) when writing colors
 *
 * @note Ensure appropriate current-limiting resistors are used on each color channel.
 *
 * @author felixthecat8a
 */

#include <Felix8A.h>

#define PIN_RED   9
#define PIN_GREEN 10
#define PIN_BLUE  11

bool commonAnode = true;

void setup() {
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
}

void loop() {
  setColor(Felix8A::Color::RED);
  delay(1000);
  setColor(Felix8A::Color::ORANGE);
  delay(1000);
  setColor(Felix8A::Color::GREEN);
  delay(1000);
  setColor(Felix8A::Color::BLUE);
  delay(1000);
  setColor(Felix8A::Color::WHITE);
  delay(1000);
}

void setColor(uint32_t color) {
  uint8_t r = Felix8A::Color::red(color);
  uint8_t g = Felix8A::Color::green(color);
  uint8_t b = Felix8A::Color::blue(color);

  if (commonAnode) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  }

  analogWrite(PIN_RED,   r);
  analogWrite(PIN_GREEN, g);
  analogWrite(PIN_BLUE,  b);
}
