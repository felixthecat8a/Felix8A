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

### Custom Color Array Setup using `Felix8A::Color` & `Felix8A::Palette`
```cpp
/***** Custom Multi-color Palette *****/
const uint32_t colorArray[] = {
  Felix8A::Color::RED,
  Felix8A::Color::YELLOW,
  Felix8A::Color::GREEN,
  Felix8A::Color::CYAN,
  Felix8A::Color::BLUE,
  Felix8A::Color::MAGENTA,
};
const Felix8A::Palette colorPalette(colorArray);
```

### Optional Preset Multi-color Palette
```cpp
const Felix8A::Palette colorPalette = Felix8A::Palette6;
```

### Initial Variables for Main Solid Color Palette
```cpp
const uint8_t numColors = colorPalette.size();
constexpr int8_t numModes = 8;
int8_t currentMode = 0;
uint8_t currentColor = 0;
bool isAnimated = false;
bool chaseAnimation = false;
bool buttonEventActivated = true;
```

### EEPROM Setup using `Felix8A::Math::wrap`
```cpp
constexpr uint8_t EEPROM_MODE_ADDR  = 0;
constexpr uint8_t EEPROM_COLOR_ADDR = 1;

void loadSettings() {
  EEPROM.get(EEPROM_MODE_ADDR, currentMode);
  EEPROM.get(EEPROM_COLOR_ADDR, currentColor);

  if (currentMode < 0 || currentMode >= numModes) { currentMode = 0; }
  if (currentColor >= numColors) { currentColor = 0; }
}

void saveSettings() {
  currentMode = Felix8A::wrap(currentMode, static_cast<int8_t>(0), numModes);
  currentColor = Felix8A::wrap(currentColor, static_cast<uint8_t>(0), numColors);

  EEPROM.update(EEPROM_MODE_ADDR, currentMode);
  EEPROM.update(EEPROM_COLOR_ADDR, currentColor);
}
```

## Color Setting Functions

### Solid Color Setting & Animated Solid Color Firefly Functions
```cpp
void firefly(uint32_t color) {
  static uint8_t brightness[NUM_LEDS] = {0};
  static int8_t direction[NUM_LEDS] = {0}; // 1 = up, -1 = down, 0 = idle
  static unsigned long lastFireflyUpdate = 0;

  if (!Time8A::every(30, lastFireflyUpdate)) return;

  for (int i = 0; i < NUM_LEDS; i++) {
    if (direction[i] == 0) {
      if (random(100) < 3) {
        brightness[i] = 10;
        direction[i] = 1;
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

    uint32_t scaled = Felix8A::Color::scale(color, brightness[i]);
    lightString->setPixelColor(i, scaled);
  }

  lightString->show();
}

void solidColor(uint32_t color, bool isAnim, bool wasUpdated) {
  if (isAnim) {
    firefly(color);
  } else if (wasUpdated) {
    lightString->fill(color);
    lightString->show();
  }
}
```

### Solid Color to White Gradient Setting Functions using `Time8A`
```cpp
void setColorGradient(uint32_t color, int step) {
  uint32_t white = Felix8A::Color::rgb(150, 150, 150);

  int count = lightString->numPixels();
  for (int i = 0; i < count; i++) {
    uint8_t phase = (i + step) % 6;

    if (phase == 0) {
      lightString->setPixelColor(i, color);
    } else if (phase == 1) {
      lightString->setPixelColor(i, Felix8A::Color::blend(color, white, 50));
    } else if (phase == 2) {
      lightString->setPixelColor(i, Felix8A::Color::blend(color, white, 100));
    } else if (phase == 3) {
      lightString->setPixelColor(i, Felix8A::Color::blend(color, white, 150));
    } else if (phase == 4) {
      lightString->setPixelColor(i, Felix8A::Color::blend(color, white, 200));
    } else {
      lightString->setPixelColor(i, Felix8A::Color::blend(color, white, 250));
    }
  }

  lightString->show();
}

void colorGradientChase(uint32_t color) {
  static unsigned long lastUpdate = 0;
  static int animStep = 0;
  int numGradientPhases = 5;

  if (Time8A::every(150, lastUpdate)) {
    setColorGradient(color, animStep);
    animStep = (animStep + 1) % numGradientPhases;
  }
}

void colorGradient(uint32_t color, bool isAnim, bool wasUpdated) {
  if (isAnim) {
    colorGradientChase(color);
  } else if (wasUpdated) {
    setColorGradient(color, 0);
  }
}
```

**Alternate White Gradient Setting Function**
```cpp
void setColorGradient(uint32_t color, int step) {
  uint32_t white = Felix8A::Color::rgb(150, 150, 150);
  uint32_t pixelColor;

  int count = lightString->numPixels();

  for (int i = 0; i < count; i++) {
    const uint8_t phase = Felix8A::wrap(i + step, 0, 6);

    switch (phase) {
      case 0: pixelColor = color; break;
      case 1: pixelColor = Felix8A::Color::blend(color, white, 50); break;
      case 2: pixelColor = Felix8A::Color::blend(color, white, 100); break;
      case 3: pixelColor = Felix8A::Color::blend(color, white, 150); break;
      case 4: pixelColor = Felix8A::Color::blend(color, white, 200); break;
      default: pixelColor = Felix8A::Color::blend(color, white, 250); break;
    }

    lightString->setPixelColor(i, pixelColor);
  }

  lightString->show();
}
```

**Another Alternate White Gradient Setting Function using Felix8A::Palette**
```cpp
void setColorGradient(uint32_t color, int step) {

  const uint32_t white = Felix8A::Color::rgb(150, 150, 150);

  const uint32_t gradientColors[] =
      {color,
       Felix8A::Color::blend(color, white, 50),
       Felix8A::Color::blend(color, white, 100),
       Felix8A::Color::blend(color, white, 150),
       Felix8A::Color::blend(color, white, 200),
       Felix8A::Color::blend(color, white, 250)};

  const Felix8A::Palette gradientPalette(gradientColors);

  const int count = lightString->numPixels();
  for (int i = 0; i < count; i++) {
    // uint8_t phase = (i + step) % gradientPalette.size();
    // lightString->setPixelColor(i, gradientPalette[phase]);
    const uint8_t phase = Felix8A::wrap(i + step, 0, (int)gradientPalette.size());
    const uint8_t t = (phase * 255) / (gradientPalette.size() - 1);
    lightString->setPixelColor(i, gradientPalette.lerp(t));
  }

  lightString->show();
}
```

## Multi-color Functions

### Multi-color Setting Functions using `Time8A`
```cpp
void multicolorTwinkle(Felix8A::Palette palette) {
  static unsigned long lastTwinkle = 0;

  if (Time8A::every(100, lastTwinkle)) {
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

void setMultiColor(Felix8A::Palette palette, int step) {
  int count = lightString->numPixels();

  for (int i = 0; i < count; i++) {
    lightString->setPixelColor(i, palette.reversed(i + step));
  }

  lightString->show();
}

void multiColorChase(Felix8A::Palette palette) {
  static unsigned long lastAnimUpdate = 0;
  static int colorStep = 0;

  if (Time8A::every(150, lastAnimUpdate)) {
    setMultiColor(palette, colorStep);
    colorStep = (colorStep + 1) % palette.count();
  }
}

void multiColor(Felix8A::Palette palette, bool isAnim, bool isChase, bool wasUpdated) {
  if (isAnim) {
    if (isChase) {
      multiColorChase(palette);
    } else {
      multicolorTwinkle(palette);
    }
  } else if (wasUpdated) {
    setMultiColor(palette, 0);
  }
}
```

## Mode Switch Code

### Lights Off Function
```cpp
void lightsOff(bool wasUpdated) {
  if (wasUpdated) {
    lightString->clear();
    lightString->show();
  }
}
```

### Set & Update Function Switch
```cpp
void updateMode(int mode, int color, bool anim, bool chase, bool stateChanged) {
  switch (mode) {
    case 0: solidColor(colorPalette[color], anim, stateChanged); break;
    case 1: colorGradient(colorPalette[color], anim, stateChanged); break;
    case 2: multiColor(colorPalette, anim, chase, stateChanged); break;
    case 3: multiColor(Felix8A::Sunset, anim, chase, stateChanged); break;
    case 4: multiColor(Felix8A::Forest, anim, chase, stateChanged); break;
    case 5: multiColor(Felix8A::Ocean, anim, chase, stateChanged); break;
    case 6: multiColor(Felix8A::Blush, anim, chase, stateChanged); break;
    case 7: multiColor(Felix8A::ChristmasLights, anim, chase, stateChanged); break;
    default: lightsOff(stateChanged); break;
  }
}
```

## Main Arduino Code

```cpp
/***** Arduino Setup *****/
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

/***** Arduino Loop *****/
void loop() {
  button.update();

  if (button.wasClicked()) {
    currentMode++;
    buttonEventActivated = true;
    saveSettings();
  }

  if (button.wasDoubleClicked()) {
    currentMode--;
    buttonEventActivated = true;
    saveSettings();
  }

  if (button.wasTripleClicked()) {
    isAnimated = !isAnimated;
    buttonEventActivated = true;
  }

  if (button.wasQuadrupleClicked()) {
    chaseAnimation = !chaseAnimation;
    buttonEventActivated = true;
  }

  if (button.wasHeld()) {
    currentColor++;
    buttonEventActivated = true;
    saveSettings();
  }

  updateMode(currentMode, currentColor, isAnimated, chaseAnimation, buttonEventActivated);
  buttonEventActivated = false;
}
```

### Alternate Loop Code (Using Event Polling)
```cpp
/***** Arduino Loop *****/
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

      case Felix8A::Button::Event::QuadrupleClick:
        chaseAnimation = !chaseAnimation;
        buttonEventActivated = true;
        break;

      case Felix8A::Button::Event::Hold:
        currentColor++;
        buttonEventActivated = true;
        saveSettings();
        break;

      default: break;
    }
  }

  updateMode(currentMode, currentColor, isAnimated, chaseAnimation, buttonEventActivated);
  buttonEventActivated = false;
}
```

#### My 100 Column Helper

```
/********1*********2*********3*********4*********5*********6*********7*********8*********9*********/
```
