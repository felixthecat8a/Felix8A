# `LightString.ino`

An Arduino sketch for controlling colors and animations on a WS2812 LED string using the `Felix8A` library.

## Sketch & Hardware Setup using `Felix8A::Button`
```cpp
#include <EEPROM.h>
#include <Felix8A.h>
#include <Adafruit_NeoPixel.h>

/***** Felix8A::Button Setup *****/
#define BUTTON_PIN 2
Felix8A::Button button(BUTTON_PIN);

/***** NeoPixel Setup *****/
#define LED_PIN A0
#define NUM_LEDS 100
Adafruit_NeoPixel* lightString = nullptr;
```

### Initial Variables for Main Solid Color Palette
```cpp
const Felix8A::Palette ColorPalette = Felix8A::Palette6;
const int numColors = ColorPalette.size();
const int numModes = 6;
int currentMode = 0;
int currentColor = 0;
bool isAnimated = false;
bool buttonEventActivated = true;
```

### EEPROM Setup using `Felix8A::Math::wrap`
```cpp
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

  int count = lightString->numPixels();

  for (int i = 0; i < count; i++) {
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
/***** Mode 0: Solid Color *****/
void solidColor(int colorIndex, bool wasUdated) {
  if (isAnimated) {
    firefly(ColorPalette[colorIndex]);
  } else if (wasUdated) {
    lightString->fill(ColorPalette[colorIndex]); lightString->show();
  }
}
```

### Solid Color to White Gradient Setting Functions using `Felix8A::Time`
```cpp
void setColorWhiteGradient(uint32_t color, int step) {
  uint32_t white = Felix8A::Color::rgb(150, 150, 150);
  uint32_t blend1 = Felix8A::Color::blend(color, white, 51);
  uint32_t blend2 = Felix8A::Color::blend(color, white, 102);
  uint32_t blend3 = Felix8A::Color::blend(color, white, 153);
  uint32_t blend3 = Felix8A::Color::blend(color, white, 204);

  int count = lightString->numPixels();
  for (int i = 0; i < count; i++) {
    uint8_t phase = (i + step) % 6;

    if (phase == 0) {
      lightString->setPixelColor(i, color);
    } else if (phase == 1) {
      lightString->setPixelColor(i, blend1);
    } else if (phase == 2) {
      lightString->setPixelColor(i, blend2);
    } else if (phase == 3) {
      lightString->setPixelColor(i, blend3);
    } else if (phase == 4) {
      lightString->setPixelColor(i, blend4);
    } else {
      lightString->setPixelColor(i, white);
    }
  }

  lightString->show();
}
/***** Mode 1: Color to White Gradient *****/
void solidColorGradient(int colorIndex, bool isAnim, bool wasUdated) {
  static unsigned long lastUpdate = 0;
  static int animStep = 0;

  if (isAnim) {
    if (wasUdated) animStep = 0;
    int numGradientPhases = 5;

    if (Felix8A::Time::every(150, lastUpdate)) {
      setColorWhiteGradient(ColorPalette[colorIndex], animStep);
      animStep = (animStep + 1) % numGradientPhases;
    }
  } else if (wasUdated) {
    setColorWhiteGradient(ColorPalette[colorIndex], 0);
  }
}
```

**Alternate White Gradient Setting Function**
```cpp
void setColorWhiteGradient(uint32_t color, int step) {
  uint32_t white = Felix8A::Color::rgb(150, 150, 150);
  uint32_t blend1 = Felix8A::Palette::blend(color, white, 51);
  uint32_t blend2 = Felix8A::Palette::blend(color, white, 102);
  uint32_t blend3 = Felix8A::Palette::blend(color, white, 153);
  uint32_t blend4 = Felix8A::Palette::blend(color, white, 204);

  int count = lightString->numPixels();
  for (int i = 0; i < count; i++) {
    uint8_t phase = (i + step) % 6;

    switch (phase) {
      case 0: lightString->setPixelColor(i, color); break;
      case 1: lightString->setPixelColor(i, blend1); break;
      case 2: lightString->setPixelColor(i, blend2); break;
      case 3: lightString->setPixelColor(i, blend3); break;
      case 4: lightString->setPixelColor(i, blend4); break;
      default: lightString->setPixelColor(i, white); break;
    }
  }

  lightString->show();
}
```

## Multi-color Functions

### Multi-color Setting Functions using `Felix8A::Time`
```cpp
void setMultiColor(Felix8A::Palette palette, int step) {
  int count = lightString->numPixels();

  for (int i = 0; i < count; i++) {
    lightString->setPixelColor(i, palette.reversed(i + step));
  }

  lightString->show();
}
/***** Mode 2: Multi-color Function *****/
void multiColor(Felix8A::Palette palette, bool isAnim, bool wasUdated) {
  static unsigned long lastAnimUpdate = 0;
  static int colorStep = 0;

  if (isAnim) {
    if (wasUdated) colorStep = 0;

    if (Felix8A::Time::every(150, lastAnimUpdate)) {
      setMultiColor(palette, colorStep);
      colorStep = (colorStep + 1) % palette.count();
    }
  } else if (wasUdated) {
    setMultiColor(palette, 0);
  }
}
```

### Multi-color Twinkle Animation Function
```cpp
/***** Mode 3: MultiColor Twinkle Animation *****/
void multicolorTwinkle(Felix8A::Palette palette) {
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
      uint32_t randColor = palette[random(palette.count())];
      lightString->setPixelColor(pixel, randColor);
    }

    lightString->show();
  }
}
```

## Mode Switch Code

### Lights Off Function
```cpp
void lightsOff(bool wasUdated) {
  if (wasUdated) {
    lightString->clear(); lightString->show();
  }
}
```

### Set & Update Function Switch
```cpp
void updateMode(int mode, int color, bool anim, bool stateChanged) {
  switch (mode) {
    case 0: solidColor(color, stateChanged); break;
    case 1: solidColorGradient(color, anim, stateChanged); break;
    case 2: multiColor(ColorPalette, anim, stateChanged); break;
    case 3: multicolorTwinkle(ColorPalette); break;
    case 4: multiColor(Felix8A::ChristmasTree, anim, stateChanged); break;
    case 5: multicolorTwinkle(Felix8A::ChristmasTree); break;
    default: lightsOff(stateChanged); break;
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
    currentMode++; buttonEventActivated = true;
    saveSettings();
  }

  if (button.wasDoubleClicked()) {
    currentMode--; buttonEventActivated = true;
    saveSettings();
  }

  if (button.wasTripleClicked()) {
    isAnimated = !isAnimated; buttonEventActivated = true;
  }

  if (button.wasHeld()) {
    currentColor++; buttonEventActivated = true;
    saveSettings();
  }

  updateMode(currentMode, currentColor, isAnimated, buttonEventActivated);
  buttonEventActivated = false;
}
```

### Loop Code (Using Event Polling)
```cpp
void loop() {
  button.update();

  Felix8A::Button::Event e;

  while ((e = button.poll()) != Felix8A::Button::Event::None) {
    switch (e) {
      case Felix8A::Button::Event::Click:
        currentMode++;
        buttonEventActivated = true;
        saveSettings();
        break;

      case Felix8A::Button::Event::DoubleClick:
        currentMode--;
        buttonEventActivated = true;
        saveSettings();
        break;

      case Felix8A::Button::Event::TripleClick:
        isAnimated = !isAnimated;
        buttonEventActivated = true;
        break;

      case Felix8A::Button::Event::Hold:
        currentColor++;
        buttonEventActivated = true;
        saveSettings();
        break;

      default:
        break;
    }
  }

  updateMode(currentMode, currentColor, isAnimated, buttonEventActivated);
  buttonEventActivated = false;
}
```

#### My 100 Column Helper

```
/********1*********2*********3*********4*********5*********6*********7*********8*********9*********/
```
