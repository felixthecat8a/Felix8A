# Three-State Quiz Show Buttons

```cpp

```

# Quiz Show Buttons

```cpp
#include <Felix8A.h>
// Player Count
constexpr uint8_t PLAYER_COUNT = 4;
// Player Button Pins
constexpr uint8_t PLAYER_BUTTON_PINS[PLAYER_COUNT] = {A0, A1, A2, A3};
// Player Indicator LED Pins
constexpr uint8_t PLAYER_LED_PINS[PLAYER_COUNT] = {2, 3, 4, 5};
// Reset Button Pin
constexpr uint8_t RESET_BUTTON_PIN = A4;
// Reset Indicator LED Pin
constexpr uint8_t RESET_LED_PIN = 6;
// Player Button Array
Felix8A::Button playerButtons[PLAYER_COUNT] = {
  Felix8A::Button(PLAYER_BUTTON_PINS[0]),
  Felix8A::Button(PLAYER_BUTTON_PINS[1]),
  Felix8A::Button(PLAYER_BUTTON_PINS[2]),
  Felix8A::Button(PLAYER_BUTTON_PINS[3])
};
// Player Indicator LED Array
Felix8A::LED playerLEDs[PLAYER_COUNT] = {
  Felix8A::LED(PLAYER_LED_PINS[0]),
  Felix8A::LED(PLAYER_LED_PINS[1]),
  Felix8A::LED(PLAYER_LED_PINS[2]),
  Felix8A::LED(PLAYER_LED_PINS[3])
};
// Reset Button Object
Felix8A::Button resetButton(RESET_BUTTON_PIN);
// Reset Indicator LED Object
Felix8A::LED resetLED(RESET_LED_PIN);
// Lockout State Variable
bool gameLocked = false;
// Reset Game Function
void resetGame() {
  // Clear the lockout state
  gameLocked = false;
  // Turn off all player LEDs
  for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
    playerLEDs[i].off();
  }
  // Turn on host LED
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
  // Reset the round if reset button is pressed
  if (resetButton.wasPressed()) {
    resetGame();
    return;
  }
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

## Quiz Show Buttons Without Library

```cpp
// Player Count
const int playerCount = 4;
// Player Button Pins
const int buttonPins[playerCount] = {A0, A1, A2, A3};
// Player Indicator LED Pins
const int ledPins[playerCount] = {2, 3, 4, 5};
// Reset Button Pin
const int resetPin = A4;
// Reset Indicator LED Pin
const int resetLED = 6;
// Lockout State Variable
bool lockedOut = false;
// Reset Game Function
void resetGame() {
    // Turn off all player LEDs
    for (int i = 0; i < playerCount; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    // Turn on host LED
    digitalWrite(resetLED, HIGH);
    // Clear the lockout state
    lockedOut = false;
}

void setup() {
  for (int i = 0; i < playerCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  pinMode(resetPin, INPUT_PULLUP);
  pinMode(resetLED, OUTPUT);

  resetGame();
}

void loop() {
  // Look for the first player to press
  if (!lockedOut) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        digitalWrite(ledPins[i], HIGH);
        digitalWrite(resetLED, LOW);
        lockedOut = true;
        break;
      }
    }
  }
  // Reset the round if reset button is pressed
  if (digitalRead(resetPin) == LOW) {
    resetGame();
    // Simple debounce delay
    delay(200);
  }
}
```
