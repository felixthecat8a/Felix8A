/**
 * @file led.ino
 * @brief Example sketch for the Felix8A RGB LED controller.
 * @note This sketch demonstrates basic color setting using the Color32 utility class.
 *       It cycles through a selection of colors defined in the Felix8A::Color class, changing the LED color every second.
 *       Circuit connections:
 *       - Red LED pin to PIN_RED (5)
 *       - Green LED pin to PIN_GREEN (6)
 *       - Blue LED pin to PIN_BLUE (9)
 *       - Common cathode of the RGB LED to GND
 * @author felixthecat8a
 */

#include <Felix8A.h>

#define PIN_RED   5
#define PIN_GREEN 6
#define PIN_BLUE  9

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
}

void setColor(int color) {
  analogWrite(PIN_RED,   Felix8A::Color::red(color));
  analogWrite(PIN_GREEN, Felix8A::Color::green(color));
  analogWrite(PIN_BLUE,  Felix8A::Color::blue(color));
}
