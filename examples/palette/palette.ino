/**
 * @file palette.ino
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

const uint32_t colorArray[] =
    {Felix8A::Color::RED,
     Felix8A::Color::ORANGE,
     Felix8A::Color::GREEN,
     Felix8A::Color::BLUE,
     Felix8A::Color::WHITE};

const Felix8A::Palette colorPalette(colorArray);

const int numColors = colorPalette.size();

#define RED_PIN   9
#define GREEN_PIN 10
#define BLUE_PIN  11
Felix8A::RGB rgb(RED_PIN, GREEN_PIN, BLUE_PIN);

bool commonAnode = true;

void setup() { rgb.begin(); }

void loop() {
  for (int i = 0; i < colorPalette.size(); i++) {
    rgb.setHex(colorPalette[i]);
    delay(1000);
  }
}
