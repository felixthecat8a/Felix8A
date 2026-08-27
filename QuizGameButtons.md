# Quiz Game Buttons

```cpp
#include <Felix8A.h>
// Player Count
constexpr uint8_t PLAYER_COUNT = 4;
// Player Button Pins
constexpr uint8_t BUTTON_PINS[PLAYER_COUNT] = {2, 3, 4, 5};
// Player Indicator LED Pins
constexpr uint8_t LED_PINS[PLAYER_COUNT] = {8, 9, 10, 11};
// Reset Button
constexpr uint8_t RESET_BUTTON_PIN = 6;
// Reset Indicator LED Pin
constexpr uint8_t RESET_LED_PIN = 7;

Felix8A::Button playerButtons[PLAYER_COUNT] =
    {Felix8A::Button(BUTTON_PINS[0]),
     Felix8A::Button(BUTTON_PINS[1]),
     Felix8A::Button(BUTTON_PINS[2]),
     Felix8A::Button(BUTTON_PINS[3])};

Felix8A::LED playerLEDs[PLAYER_COUNT] =
    {Felix8A::LED(LED_PINS[0]),
     Felix8A::LED(LED_PINS[1]),
     Felix8A::LED(LED_PINS[2]),
     Felix8A::LED(LED_PINS[3])};

Felix8A::Button resetButton(RESET_BUTTON_PIN);

Felix8A::LED resetLED(RESET_LED_PIN);

bool gameLocked = false;

void resetGame() {
  gameLocked = false;

  for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
    playerLEDs[i].off();
  }

  resetLED.on();
}

void setup() {
  for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
    playerButtons[i].begin();
    playerLEDs[i].begin();
  }

  resetButton.begin();
  resetLED.begin();

  resetGame();
}

void loop() {
  // Update player buttons
  for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
    playerButtons[i].update();
  }
  // Update reset buttons
  resetButton.update();
  // Reset the round
  if (resetButton.wasPressed()) { resetGame(); }
  // Look for the first player to press
  if (!gameLocked) {
    for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
      if (playerButtons[i].wasPressed()) {
        playerLEDs[i].on();
        resetLED.off();
        gameLocked = true;
        break;
      }
    }
  }
}
```
