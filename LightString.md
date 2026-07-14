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

### `Felix8A` Preset Solid Color Palettes
```cpp
// const Felix8A::Palette ColorPalette = Felix8A::Palette36;
// const Felix8A::Palette ColorPalette = Felix8A::Palette24;
const Felix8A::Palette ColorPalette = Felix8A::Palette12;
// const Felix8A::Palette ColorPalette = Felix8A::Palette6;
// const Felix8A::Palette ColorPalette = Felix8A::Palette3;
// const Felix8A::Palette ColorPalette = Felix8A::ChristmasTree;
// const Felix8A::Palette ColorPalette = Felix8A::Sunset;
// const Felix8A::Palette ColorPalette = Felix8A::Forest;
// const Felix8A::Palette ColorPalette = Felix8A::Ocean;
// const Felix8A::Palette ColorPalette = Felix8A::Blush;
```

### Initial Variables for Solid Color Palette
```cpp
/***** Initial Variables for Solid Color Palette *****/
const int numColors = ColorPalette.size();
const int numModes = 4;
int currentMode = 0;
int currentColor = 0;
bool isAnimated = false;
bool stateUpdated = true;
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
}
```

## Color Setting Functions

### Solid Color Setting & Animated Solid Color Firefly Functions
```cpp
void firefly(uint32_t baseColor) {
  static uint8_t brightness[NUM_LEDS] = {0};
  static int8_t direction[NUM_LEDS] = {0}; // 1 = up, -1 = down, 0 = idle
  static unsigned long lastFireflyUpdate = 0;

  if (!Felix8A::Time::every(30, lastFireflyUpdate)) return;

  for (int i = 0; i < lightString->numPixels(); i++) {
    if (direction[i] == 0) {
      if (random(100) < 3) {
        direction[i] = 1;
        brightness[i] = 10;
      }
    }

    if (direction[i] != 0) {
      brightness[i] += direction[i] * 10;

      if (brightness[i] >= 200) {
        brightness[i] = 200;
        direction[i] = -1;
      }

      if (brightness[i] <= 0) {
        brightness[i] = 0;
        direction[i] = 0;
      }
    }

    uint32_t scaled = Felix8A::Color::scale(baseColor, brightness[i]);
    lightString->setPixelColor(i, scaled);
  }

  lightString->show();
}

void solidColor() {
  if (isAnimated) {
    firefly(ColorPalette[currentColor]);
  } else if (stateUpdated) {
    lightString->fill(ColorPalette[currentColor]); lightString->show();
  }
}
```

### Solid Color to White Gradient Setting Functions using `Felix8A::Time`
```cpp
void setColorWhiteGradient(uint32_t color, int step) {
  uint32_t white = Felix8A::Color::rgb(150, 150, 150);
  uint32_t blend1 = Felix8A::Color::blend(color, white, 64);
  uint32_t blend2 = Felix8A::Color::blend(color, white, 128);
  uint32_t blend3 = Felix8A::Color::blend(color, white, 192);

  for (int i = 0; i < lightString->numPixels(); i++) {
    uint8_t phase = (i + step) % 5;

    if (phase == 0) {
      lightString->setPixelColor(i, color);
    } else if (phase == 1) {
      lightString->setPixelColor(i, blend1);
    } else if (phase == 2) {
      lightString->setPixelColor(i, blend2);
    } else if (phase == 3) {
      lightString->setPixelColor(i, blend3);
    } else {
      lightString->setPixelColor(i, white);
    }
  }

  lightString->show();
}

void solidColorGradient() {
  static unsigned long lastUpdate = 0;
  static int animStep = 0;

  if (isAnimated) {
    if (stateUpdated) animStep = 0;
    int numGradientPhases = 5;

    if (Felix8A::Time::every(150, lastUpdate)) {
      setColorWhiteGradient(ColorPalette[currentColor], animStep);
      animStep = (animStep + 1) % numGradientPhases;
    }
  } else if (stateUpdated) {
    setColorWhiteGradient(ColorPalette[currentColor], 0);
  }
}
```

**Alternate White Gradient Setting Function**
```cpp
void setColorWhiteGradient(uint32_t color, int step) {
  uint32_t white = Felix8A::Color::rgb(150, 150, 150);
  uint32_t blend1 = Felix8A::Palette::blend(color, white, 64);
  uint32_t blend2 = Felix8A::Palette::blend(color, white, 128);
  uint32_t blend3 = Felix8A::Palette::blend(color, white, 192);

  for (int i = 0; i < lightString->numPixels(); i++) {
    uint8_t phase = (i + step) % 5;

    switch (phase) {
      case 0: lightString->setPixelColor(i, color); break;
      case 1: lightString->setPixelColor(i, blend1); break;
      case 2: lightString->setPixelColor(i, blend2); break;
      case 3: lightString->setPixelColor(i, blend3); break;
      default: lightString->setPixelColor(i, white); break;
    }
  }

  lightString->show();
}
```

## Multi-color Functions

### Multi-color Setting Functions using `Felix8A::Time`
```cpp
void setMultiColor(int step) {
  for (int i = 0; i < lightString->numPixels(); i++) {
    lightString->setPixelColor(i, ColorPalette.reversed(i + step));
  }

  lightString->show();
}

void multiColor() {
  static unsigned long lastAnimUpdate = 0;
  static int colorStep = 0;

  if (isAnimated) {
    if (stateUpdated) colorStep = 0;

    if (Felix8A::Time::every(150, lastAnimUpdate)) {
      setMultiColor(colorStep);
      colorStep = (colorStep + 1) % numColors;
    }
  } else if (stateUpdated) {
    setMultiColor(0);
  }
}
```

### Multi-color Twinkle Animation Function
```cpp
void multicolorTwinkle() {
  static unsigned long lastTwinkle = 0;

  if (Felix8A::Time::every(100, lastTwinkle)) {
    int count = lightString->numPixels();

    for (int i = 0; i < count; i++) {
      uint32_t color = lightString->getPixelColor(i);
      lightString->setPixelColor(i, Felix8A::Color::scale(color, 220));
    }

    int newPixels = random(1, 4);

    for (int i = 0; i < newPixels; i++) {
      int pixel = random(count);
      uint32_t randColor = ColorPalette[random(ColorPalette.count())];
      lightString->setPixelColor(pixel, randColor);
    }

    lightString->show();
  }
}
```

## Mode Switch Code

### Lights Off Function
```cpp
void lightsOff() {
  if (stateUpdated) {
    lightString->clear(); lightString->show();
  }
}
```

### Set & Update Function Switch
```cpp
void updateMode() {
  switch (currentMode) {
    case 0: solidColor(); break;
    case 1: solidColorGradient(); break;
    case 2: multiColor(); break;
    case 3: multicolorTwinkle(); break;
    default: lightsOff(); break;
  }
}
```

## Main Arduino Code

### Setup Code
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
  lightString->setBrightness(51);
  lightString->show();
}
```

### Loop Code
```cpp
void loop() {
  button.update();

  if (button.wasClicked()) {
    currentMode++;
    stateUpdated = true;
    saveSettings();
  }

  if (button.wasDoubleClicked()) {
    currentMode--;
    stateUpdated = true;
    saveSettings();
  }

  if (button.wasTripleClicked()) {
    isAnimated = !isAnimated;
    stateUpdated = true;
  }

  if (button.wasHeld()) {
    currentColor++;
    stateUpdated = true;
    saveSettings();
  }

  updateMode();
  stateUpdated = false;
}
```

### Loop Code (Using Event Polling)
```cpp
void loop() {
  button.update();

  FelixTheCatLED::Button::Event e;

  while ((e = button.poll()) != FelixTheCatLED::Button::Event::None) {
    switch (e) {
      case FelixTheCatLED::Button::Event::Click:
        currentMode++;
        stateUpdated = true;
        saveSettings();
        break;

      case FelixTheCatLED::Button::Event::DoubleClick:
        currentMode--;
        stateUpdated = true;
        saveSettings();
        break;

      case FelixTheCatLED::Button::Event::TripleClick:
        isAnimated = !isAnimated;
        stateUpdated = true;
        break;

      case FelixTheCatLED::Button::Event::Hold:
        currentColor++;
        stateUpdated = true;
        saveSettings();
        break;

      default:
        break;
    }
  }

  updateMode();
  stateUpdated = false;
}
```

```
/********1*********2*********3*********4*********5*********6*********7*********/
```
