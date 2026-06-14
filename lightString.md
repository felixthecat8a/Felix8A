# `LightString.ino`

An Arduino sketch for controlling colors and animations on a WS2812 LED string using the `Felix8A` and `FelixTheCatLED` libraries.

Note: Math and Time helpers are in development

## Sketch & Hardware Setup using `FelixTheCatLED::Button`

```cpp
#include <EEPROM.h>
#include <FelixTheCatLED.h>
#include <Felix8A.h>
#include <Adafruit_NeoPixel.h>

/***** FelixTheCatLED::Button Setup *****/
#define BUTTON_PIN 2
FelixTheCatLED::Button button(BUTTON_PIN);

/***** NeoPixel Setup *****/
#define LED_PIN A0
#define NUM_LEDS 100
Adafruit_NeoPixel* lightString = nullptr;
```

### Initial Variables using `Felix8A::Color` & `Felix8A::Palette`

```cpp
/***** Classic Christmas Tree Light Color Palette *****/
constexpr uint32_t ledWhite = Felix8A::Color::hex(150, 150, 150);
constexpr uint32_t ledRed = Felix8A::Color::RED;
constexpr uint32_t ledAmber = Felix8A::Color::ORANGE;
constexpr uint32_t ledGreen = Felix8A::Color::GREEN;
constexpr uint32_t ledBlue = Felix8A::Color::BLUE;
constexpr uint32_t ledColor[] = { ledRed, ledAmber, ledGreen, ledBlue };
// constexpr int numColors = sizeof(ledColor) / sizeof(ledColor[0]);
constexpr Felix8A::Palette colorPalette(ledColor);
constexpr int numColors = colorPalette.size();

/***** Initial Variables *****/
constexpr int numModes = 7;
int currentMode = 0;
int currentColor = 0;
bool settingsUpdated = true;
int chaseStep = 0;
const unsigned long chaseInterval = 125;
unsigned long lastUpdate = 0;
```

### EEPROM Setup using `Felix8A::Math::wrap`

```cpp
/***** EEPROM  Setup *****/
#define EEPROM_MODE_ADDR 0
#define EEPROM_COLOR_ADDR 1

void loadSettings() {
  EEPROM.get(EEPROM_MODE_ADDR, currentMode);
  EEPROM.get(EEPROM_COLOR_ADDR, currentColor);
  if (currentMode < 0 || currentMode >= numModes) currentMode = 0;
  if (currentColor < 0 || currentColor >= numColors) currentColor = 0;
}

void saveSettings() {
  currentMode = Felix8A::Math::wrap(currentMode, 0, numModes);
  currentColor = Felix8A::Math::wrap(currentColor, 0, numColors);
  EEPROM.update(EEPROM_MODE_ADDR, currentMode);
  EEPROM.update(EEPROM_COLOR_ADDR, currentColor);
  settingsUpdated = true;
}
```

## Color Setting Functions

### Solid Color Setting Functions

```cpp
void lightsOff() {
  if (settingsUpdated) {
    lightString->clear(); lightString->show();
  }
}

void solidColor() {
  if (settingsUpdated) {
    // lightString->fill(ledColor[currentColor]); lightString->show();
    lightString->fill(colorPalette[currentColor]); lightString->show();
  }
}
```

### Color to White Gradient Setting Functions using `Felix8A::Time`

```cpp
void setColorWhiteGradient(int step) {
  // uint32_t color = ledColor[currentColor];
  uint32_t color = colorPalette[currentColor];
  uint32_t blend1 = Felix8A::Color::blend(color, ledWhite, 64);
  uint32_t blend2 = Felix8A::Color::blend(color, ledWhite, 128);
  uint32_t blend3 = Felix8A::Color::blend(color, ledWhite, 192);

  for (int i = 0; i < lightString->numPixels(); i++) {
    uint8_t phase = (i + step) % 5;

    // if (phase == 0) {
    //   lightString->setPixelColor(i, color);
    // } else if (phase == 1) {
    //   lightString->setPixelColor(i, blend1);
    // } else if (phase == 2) {
    //   lightString->setPixelColor(i, blend2);
    // } else if (phase == 3) {
    //   lightString->setPixelColor(i, blend3);
    // } else {
    //   lightString->setPixelColor(i, ledWhite);
    // }

    switch (phase) {
      case 0: lightString->setPixelColor(i, color); break;
      case 1: lightString->setPixelColor(i, blend1); break;
      case 2: lightString->setPixelColor(i, blend2); break;
      case 3: lightString->setPixelColor(i, blend3); break;
      default: lightString->setPixelColor(i, ledWhite); break;
    }
  }

  lightString->show();
}

void colorGradient(bool isAnimated) {
  if (isAnimated) {
    if (settingsUpdated) chaseStep = 0;

    if (Felix8A::Time::every(chaseInterval, lastUpdate)) {
      setColorWhiteGradient(chaseStep);
      chaseStep = (chaseStep + 1) % 5;
    }
  } else if (settingsUpdated) {
    setColorWhiteGradient(0);
  }
}
```

### Multiple Color Setting Functions using `Felix8A::Time`

```cpp
void setMultiColor(int step) {
  for (int i = 0; i < lightString->numPixels(); i++) {
    // lightString->setPixelColor(i, ledColor[(i + step) % numColors]);
    lightString->setPixelColor(i, colorPalette.reversed(i + step));
  }

  lightString->show();
}

void multiColor(bool isAnimated) {
  if (isAnimated) {
    if (settingsUpdated) chaseStep = 0;

    if (Felix8A::Time::every(chaseInterval, lastUpdate)) {
      setMultiColor(chaseStep);
      chaseStep = (chaseStep + 1) % numColors;
    }
  } else if (settingsUpdated) {
    setMultiColor(0);
  }
}
```

### Firefly Animation Function

```cpp
void fireflyLights() {
  static uint8_t brightness[NUM_LEDS] = {0};
  static int8_t direction[NUM_LEDS] = {0}; // 1 = up, -1 = down, 0 = idle
  static unsigned long lastFireflyUpdate = 0;
  if (!Felix8A::Time::every(30, lastFireflyUpdate)) return;
  for (int i = 0; i < lightString->numPixels(); i++) {
    if (direction[i] == 0) {
      if (random(100) < 3) {
        direction[i] = 1; brightness[i] = 10;
      }
    }
    if (direction[i] != 0) {
      brightness[i] += direction[i] * 10;
      if (brightness[i] >= 200) {
        brightness[i] = 200; direction[i] = -1;
      }
      if (brightness[i] <= 0) {
        brightness[i] = 0; direction[i] = 0;
      }
    }
    uint32_t baseColor = colorPalette[currentColor];
    uint32_t scaled = Felix8A::Color::scale(baseColor, brightness[i]);
    lightString->setPixelColor(i, scaled);
  }
  lightString->show();
}
```

### Twinkle Animation Function

```cpp
void twinkleLights() {
  static unsigned long lastTwinkle = 0;

  if (Felix8A::Time::every(100, lastTwinkle)) {
    int count = lightString->numPixels();

    for (int i = 0; i < count; i++) {
      uint32_t c = lightString->getPixelColor(i);
      uint8_t r = ((c >> 16) & 0xFF) * 220 / 255;
      uint8_t g = ((c >> 8) & 0xFF) * 220 / 255;
      uint8_t b = (c & 0xFF) * 220 / 255;
      lightString->setPixelColor(i, lightString->Color(r, g, b));
    }

    int newTwinkles = random(1, 3);

    for (int i = 0; i < newTwinkles; i++) {
      int pixel = random(count);
      // lightString->setPixelColor(pixel, ledColor[random(numColors)]);
      lightString->setPixelColor(pixel, colorPalette[random(numColors)]);
    }

    if (random(9) == 0) {
      int pixel = random(count);
      lightString->setPixelColor(pixel, ledWhite);
    }

    lightString->show();
  }
}
```

## Set & Update Functions

```cpp
void updateMode() {
  switch (currentMode) {
    case 0: solidColor(); break;
    case 1: colorGradient(false); break;
    case 2: colorGradient(true); break;
    case 3: multiColor(false); break;
    case 4: multiColor(true); break;
    case 5: twinkleLights(); break;
    case 6: fireflyLights(); break;
    default: lightsOff(); break;
  }
}
```

### Main Code

```cpp
void setup() {
  loadSettings();

  button.begin();
  button.setHoldTime(750);
  button.setMultiClickTime(250);

  if (button.isDown()) {
    lightString = new Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);
  } else {
    lightString = new Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
  }
  lightString->begin();
  lightString->setBrightness(50);
  lightString->show();
}

void loop() {
  button.update();

  if (button.wasClicked()) {
    currentMode++;
    saveSettings();
  }

  if (button.wasDoubleClicked()) {
    currentMode--;
    saveSettings();
  }

  if (button.wasTripleClicked()) {
    currentMode = 0; currentColor = 0;
    saveSettings();
  }

  if (button.wasHeld()) {
    currentColor++;
    saveSettings();
  }

  updateMode();
  settingsUpdated = false;
}
```
