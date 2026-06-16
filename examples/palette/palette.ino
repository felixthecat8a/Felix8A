/**
 * @file palette.ino
 * @brief Example sketch for the Felix8A RGB LED controller.
 * @note This sketch demonstrates basic color setting using the Palette class.
 *       It cycles through a selection of colors defined in the Felix8A::Color class, changing the LED color every second.
 *       Circuit connections:
 *       - Red LED pin to PIN_RED (5)
 *       - Green LED pin to PIN_GREEN (6)
 *       - Blue LED pin to PIN_BLUE (9)
 *       - Common cathode of the RGB LED to GND
 * @author felixthecat8a
 */

#include <Felix8A.h>

const uint32_t colorArray[] = {
    Felix8A::Color::RED,
    Felix8A::Color::ORANGE,
    Felix8A::Color::GREEN,
    Felix8A::Color::BLUE,
    Felix8A::Color::WHITE
};

const Felix8A::Palette ColorPalette(colorArray);
const int numColors = ColorPalette.size();

#define PIN_RED   9
#define PIN_GREEN 10
#define PIN_BLUE  11

void setup() {
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
}

void loop() {
  for (int i = 0; i < ColorPalette.size(); i++) {
    setColor(ColorPalette[i]);
    delay(1000);
  }
}

void setColor(int color) {
  analogWrite(PIN_RED,   Felix8A::Color::red(color));
  analogWrite(PIN_GREEN, Felix8A::Color::green(color));
  analogWrite(PIN_BLUE,  Felix8A::Color::blue(color));
}
