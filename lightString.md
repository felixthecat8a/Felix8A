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

### Custom Multi-color Setup using `Felix8A::Color` & `Felix8A::Palette`
```cpp
/***** Classic Christmas Tree Light Multi-color Palette *****/
constexpr uint32_t ledRed = Felix8A::Color::RED;
constexpr uint32_t ledOrange = Felix8A::Color::ORANGE;
constexpr uint32_t ledGreen = Felix8A::Color::GREEN;
constexpr uint32_t ledBlue = Felix8A::Color::BLUE;
constexpr uint32_t colorArray[] = { ledRed, ledOrange, ledGreen, ledBlue };
constexpr Felix8A::Palette MultiColorPalette(colorArray);

/***** Preset Classic Christmas Tree Light Multi-color Palette *****/
//constexpr Felix8A::Palette MultiColorPalette(Felix8A::Sets::ChristmasTree5);

/***** Animation Speed for Color Array *****/
const unsigned long animInterval = 200;
```

### Initial Variables for Solid Color Palette
```cpp
/***** Initial Variables for Solid Color Palette *****/
const int numColors = Felix8A::Palette6.size();
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

  stateUpdated = true;
}
```

## Color Setting Functions

### Solid Color Setting & Animated Solid Color Firefly Functions
```cpp
void firefly() {
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

    uint32_t baseColor = Felix8A::Palette6[currentColor];
    uint32_t scaled = Felix8A::Color::scale(baseColor, brightness[i]);
    lightString->setPixelColor(i, scaled);
  }

  lightString->show();
}

void solidColor() {
  if (isAnimated) {
    firefly();
  } else if (stateUpdated) {
    lightString->fill(Felix8A::Palette6[currentColor]); lightString->show();
  }
}
```

### Solid Color to White Gradient Setting Functions using `Felix8A::Time`
```cpp
void setColorWhiteGradient(int step) {
  uint32_t color = Felix8A::Palette6[currentColor];
  uint32_t white = Felix8A::Color::GRAY;
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
      setColorWhiteGradient(animStep);
      animStep = (animStep + 1) % numGradientPhases;
    }
  } else if (stateUpdated) {
    setColorWhiteGradient(0);
  }
}
```

**Alternate White Gradient Setting Function**
```cpp
void setColorWhiteGradient(int step) {
  uint32_t color = Felix8A::Palette6[currentColor];
  uint32_t white = Felix8A::Color::GRAY;
  uint32_t blend1 = Felix8A::Color::blend(color, white, 64);
  uint32_t blend2 = Felix8A::Color::blend(color, white, 128);
  uint32_t blend3 = Felix8A::Color::blend(color, white, 192);

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

### Custom Multiple Color Setting Functions using `Felix8A::Time`
```cpp
void setMultiColor(int step) {
  for (int i = 0; i < lightString->numPixels(); i++) {
    lightString->setPixelColor(i, MultiColorPalette.reversed(i + step));
  }
  lightString->show();
}
void multiColor() {
  static unsigned long lastAnimUpdate = 0;
  static int colorStep = 0;
  if (isAnimated) {
    if (stateUpdated) colorStep = 0;
    if (Felix8A::Time::every(animInterval, lastAnimUpdate)) {
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
void multiColorTwinkle() {
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
      int rand = random(MultiColorPalette.count());
      lightString->setPixelColor(pixel, MultiColorPalette[rand]);
    }

    lightString->show();
  }
}
```

### Light Off Function
```cpp
void lightsOff() {
  if (stateUpdated) {
    lightString->clear(); lightString->show();
  }
}
```

## Set & Update Functions
```cpp
void updateMode() {
  switch (currentMode) {
    case 0: solidColor(); break;
    case 1: solidColorGradient(); break;
    case 2: multiColor(); break;
    case 3: multiColorTwinkle(); break;
    default: lightsOff(); break;
  }
}
```

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
    saveSettings();
  }

  if (button.wasDoubleClicked()) {
    currentMode--;
    saveSettings();
  }

  if (button.wasTripleClicked()) {
    isAnimated = !isAnimated;
    stateUpdated = true;
  }

  if (button.wasHeld()) {
    currentColor++;
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
        saveSettings();
        break;

      case FelixTheCatLED::Button::Event::DoubleClick:
        currentMode--;
        saveSettings();
        break;

      case FelixTheCatLED::Button::Event::TripleClick:
        isAnimated = !isAnimated;
        stateUpdated = true;
        break;

      case FelixTheCatLED::Button::Event::Hold:
        currentColor++;
        saveSettings();
        break;

      default:
        break;
    }
  }

  updateMode();
  stateUpdated = false;
}
/********1*********2*********3*********4*********5*********6*********7*********/
```
