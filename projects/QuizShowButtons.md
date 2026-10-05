# Three-State Quiz Show Buttons

```cpp
#include <Felix8A.h>

class StatusLight {
public:
  enum class Color : uint8_t { Off, Red, Amber, Green };

  StatusLight(uint8_t redPin, uint8_t amberPin, uint8_t greenPin, bool activeLow = false)
      : _red(redPin, activeLow), _amber(amberPin, activeLow), _green(greenPin, activeLow) {}

  void begin() {
    _red.begin();
    _amber.begin();
    _green.begin();
    off();
  }

  void set(Color color) {
    off();

    switch (color) {
      case Color::Red: _red.on(); break;
      case Color::Amber: _amber.on(); break;
      case Color::Green: _green.on(); break;
      case Color::Off:
      default: break;
    }

    _current = color;
  }

  void red() { set(Color::Red); }
  void amber() { set(Color::Amber); }
  void green() { set(Color::Green); }
  void off() { set(Color::Off); }

  Color state() const { return _current; }

private:
  Felix8A::LED _red;
  Felix8A::LED _amber;
  Felix8A::LED _green;

  Color _current = Color::Off;
};

// Player Count
constexpr uint8_t PLAYER_COUNT = 4;
// Player Button Pins
constexpr uint8_t PLAYER_BUTTON_PINS[PLAYER_COUNT] = {A0, A1, A2, A3};
// Player Indicator LED Pins
constexpr uint8_t PLAYER_LED_PINS[PLAYER_COUNT] = {2, 3, 4, 5};
// Host Button Pin
constexpr uint8_t HOST_BUTTON_PIN = A4;
// Host Indicator LED Pin
constexpr uint8_t HOST_LED_PIN = 6;
// Status Light Pins
constexpr uint8_t STATUS_RED_PIN = 7;
constexpr uint8_t STATUS_AMBER_PIN = 8;
constexpr uint8_t STATUS_GREEN_PIN = 9;
// Player Button Array
Felix8A::Button playerButtons[PLAYER_COUNT] =
    {Felix8A::Button(PLAYER_BUTTON_PINS[0]),
     Felix8A::Button(PLAYER_BUTTON_PINS[1]),
     Felix8A::Button(PLAYER_BUTTON_PINS[2]),
     Felix8A::Button(PLAYER_BUTTON_PINS[3])};
// Player Indicator LED Array
Felix8A::LED playerLEDs[PLAYER_COUNT] =
    {Felix8A::LED(PLAYER_LED_PINS[0]),
     Felix8A::LED(PLAYER_LED_PINS[1]),
     Felix8A::LED(PLAYER_LED_PINS[2]),
     Felix8A::LED(PLAYER_LED_PINS[3])};
// Host Button
Felix8A::Button hostButton(HOST_BUTTON_PIN);
// Host Indicator LED
Felix8A::LED hostLED(HOST_LED_PIN);
// Status Indicator
StatusLight statusLight(STATUS_RED_PIN, STATUS_AMBER_PIN, STATUS_GREEN_PIN, false);

// Game State

enum class GameState : uint8_t { Question, Play, Locked };

GameState gameState = GameState::Question;

// Game State Management

void setGameState(GameState newState) {
  gameState = newState;

  switch (gameState) {
    case GameState::Question:
      for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
        playerLEDs[i].off();
      }
      statusLight.amber();
      hostLED.on();
      break;

    case GameState::Play:
      for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
        playerLEDs[i].off();
      }
      statusLight.green();
      hostLED.off();
      break;

    case GameState::Locked:
      statusLight.red();
      hostLED.on();
      break;
  }
}

// Setup
void setup() {
  for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
    playerButtons[i].begin();
    playerLEDs[i].begin();
  }

  hostButton.begin();
  hostLED.begin();
  statusLight.begin();

  setGameState(GameState::Question);
}
// Main Loop
void loop() {
  bool playerPressed[PLAYER_COUNT] = {};

  for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
    playerButtons[i].update();
    playerPressed[i] = playerButtons[i].wasPressed();
  }
  // Update host button
  hostButton.update();
  // Check host button
  if (hostButton.wasPressed()) {
    switch (gameState) {
      case GameState::Question: setGameState(GameState::Play); break;
      case GameState::Locked: setGameState(GameState::Question); break;
      case GameState::Play: break;
    }
    return;
  }
  // Only accept player presses during play
  if (gameState != GameState::Play) { return; }
  // Look for the first player to press
  for (uint8_t i = 0; i < PLAYER_COUNT; i++) {
    if (playerPressed[i]) {
      playerLEDs[i].on();
      setGameState(GameState::Locked);
      break;
    }
  }
}
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
