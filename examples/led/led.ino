/**
 * @file led.ino
 * @brief Example sketch for the Felix8A LED controller.
 *
 * @details
 * This sketch demonstrates the LED, PWM & Time classes
 *
 * @section wiring Wiring
 * * LED pin (6)
 * * PWM pin (5)
 *
 * @note Ensure appropriate current-limiting resistors are used on each LED channel.
 *
 * @author felixthecat8a
 */
#include <Felix8A.h>

#define LED_PIN 6
Felix8A::LED led(LED_PIN);

const unsigned long blinkInterval = 1000;
unsigned long lastBlink = 0;

#define PWM_PIN 5
Felix8A::PWM pwm(PWM_PIN);

int brightness = 0;
int fadeAmount = 5;

void setup() {
  led.begin();
  led.on();
  pwm.begin();
}

void loop() {
  if (Felix8A::Time::every(blinkInterval, lastBlink)) {
    led.toggle();
  }

  fade();
}

void fade() {
  static unsigned long lastUpdate = 0; // persists between calls

  if (Felix8A::Time::every(50, lastUpdate)) {
    pwm.setBrightness(brightness);
    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount; // Reverse direction at limits
    }
  }
}