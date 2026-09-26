# `LightString.ino`

An Arduino sketch for controlling colors and animations on a WS2812 LED string using the `Felix8A` library.

## Sketch & Hardware Setup

```cpp
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Felix8A.h>
/***** Felix8A::Button Setup *****/
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint16_t BUTTON_DEBOUNCE = 25;
Felix8A::Button button(BUTTON_PIN, BUTTON_DEBOUNCE);
constexpr unsigned long BUTTON_HOLD_TIME = 750;
constexpr unsigned long BUTTON_MULTI_CLICK_TIME = 250;
/***** NeoPixel Setup *****/
constexpr uint8_t LED_PIN = A0;
constexpr uint16_t NUM_LEDS = 100;
Adafruit_NeoPixel *lightString = nullptr;
constexpr uint8_t LED_BRIGHTNESS = 51;
/***** Mode Setup with Default Colors *****/
const Felix8A::Palette colorPalette = Felix8A::Palette6;
const uint8_t numColors = colorPalette.size();
constexpr int8_t numModes = 9;
int8_t currentMode = 0;
uint8_t currentColor = 0;
bool isAnimated = false;
bool chaseAnimation = false;
bool eventActivated = true;
/***** Gradient Animation Setup *****/
constexpr unsigned long GRADIENT_INTERVAL = 150;
unsigned long lastGradientUpdate = 0;
uint8_t gradientStep = 0;
/***** Multicolor Animation Setup *****/
constexpr unsigned long TWINKLE_INTERVAL = 100;
constexpr unsigned long CHASE_INTERVAL = 150;
unsigned long lastChaseUpdate = 0;
uint8_t chaseStep = 0;
/***** Firefly Animation Setup *****/
constexpr unsigned long FIREFLY_INTERVAL = 50;
unsigned long lastFireflyUpdate = 0;
uint8_t fireflyBrightness[NUM_LEDS] = {};
int8_t fireflyDirection[NUM_LEDS] = {};
```

### Custom Color Palette Setup

```cpp
/***** Custom Multi-color Palette *****/
constexpr uint32_t CUSTOM_COLORS[] = {
  Felix8A::Color::RED,
  Felix8A::Color::ORANGE,
  Felix8A::Color::GREEN,
  Felix8A::Color::BLUE
};

const Felix8A::Palette customPalette(CUSTOM_COLORS);
```

### EEPROM Setup & Setting Update Functions
```cpp
constexpr uint8_t EEPROM_MODE_ADDR = 0;
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

void nextMode() {
  currentMode++;
  saveSettings();
}

void previousMode() {
  currentMode--;
  saveSettings();
}

void nextColor() {
  currentColor++;
  saveSettings();
}
```

## Light Color Setting Functions

### Solid Color Setting, Animated Firefly, Gradient & Gradient Chase Functions

```cpp
void setColorGradient(uint32_t color, int step) {
  const int count = lightString->numPixels();
  constexpr uint8_t GRADIENT_PHASES = 5;
  const uint32_t white = Felix8A::Color::rgb(150, 150, 150);

  for (int i = 0; i < count; ++i) {
    // uint8_t phase = (i + step) % GRADIENT_PHASES;
    const uint8_t phase = Felix8A::wrap(i + step, 0, static_cast<int>(GRADIENT_PHASES));

    uint32_t pixelColor;

    // if (phase == 0) {
    //   pixelColor = color;
    // } else if (phase == 1) {
    //   pixelColor = Felix8A::Color::blend(color, white, 50);
    // } else if (phase == 2) {
    //   pixelColor = Felix8A::Color::blend(color, white, 100);
    // } else if (phase == 3) {
    //   pixelColor = Felix8A::Color::blend(color, white, 150);
    // } else {
    //   pixelColor = Felix8A::Color::blend(color, white, 200);
    // }

    switch (phase) {
      case 0: pixelColor = color; break;
      case 1: pixelColor = Felix8A::Color::blend(color, white, 50); break;
      case 2: pixelColor = Felix8A::Color::blend(color, white, 100); break;
      case 3: pixelColor = Felix8A::Color::blend(color, white, 150); break;
      default: pixelColor = Felix8A::Color::blend(color, white, 200); break;
    }

    lightString->setPixelColor(i, pixelColor);
  }

  lightString->show();
}

void colorGradientChase(uint32_t color) {
  if (!Time8A::every(GRADIENT_INTERVAL, lastGradientUpdate)) { return; }

  setColorGradient(color, gradientStep);

  gradientStep = Felix8A::wrap(gradientStep + 1, 0, 5);
}

void firefly(uint32_t color) {
  if (!Time8A::every(FIREFLY_INTERVAL, lastFireflyUpdate)) { return; }

  for (uint16_t i = 0; i < NUM_LEDS; ++i) {
    if (fireflyDirection[i] == 0) {
      if (random(100) < 1) {
        fireflyBrightness[i] = 10;
        fireflyDirection[i] = 1;
      }
    }

    if (fireflyDirection[i] != 0) {
      int newBrightness = fireflyBrightness[i] + fireflyDirection[i] * 10;
      fireflyBrightness[i] = Felix8A::clamp(newBrightness, 0, 200);
      if (fireflyBrightness[i] >= 200) { fireflyDirection[i] = -1; }
      if (fireflyBrightness[i] == 0) { fireflyDirection[i] = 0; }
    }

    const uint32_t scaled = Felix8A::Color::scale(color, fireflyBrightness[i]);

    lightString->setPixelColor(i, scaled);
  }

  lightString->show();
}

void solidColor(uint32_t color, bool animated, bool chase, bool wasUpdated) {
  if (animated) {
    if (chase) {
      colorGradientChase(color);
    } else {
      firefly(color);
    }
  } else if (wasUpdated) {
    if (chase) {
      setColorGradient(color, 0);
    } else {
      lightString->fill(color);
      lightString->show();
    }
  }
}
```

### Multi-color Setting Functions
```cpp
void multicolorTwinkle(const Felix8A::Palette& palette) {

  if (!Time8A::every(TWINKLE_INTERVAL, lastGradientUpdate)) { return; }

  const int count = lightString->numPixels();

  for (int i = 0; i < count; ++i) {
    uint32_t color = lightString->getPixelColor(i);
    lightString->setPixelColor(i, Felix8A::Color::scale(color, 220));
  }

  const int newPixels = random(1, 4);

  for (int i = 0; i < newPixels; ++i) {
    const int pixel = random(count);
    const uint32_t randomColor = palette[random(palette.size())];
    lightString->setPixelColor(pixel, randomColor);
  }

  lightString->show();
}

void setMultiColor(const Felix8A::Palette& palette, int step) {
  const int count = lightString->numPixels();

  for (int i = 0; i < count; ++i) {
    lightString->setPixelColor(i, palette.reversed(i + step));
  }

  lightString->show();
}

void multiColorChase(const Felix8A::Palette& palette) {
  if (!Time8A::every(CHASE_INTERVAL, lastChaseUpdate)) { return; }

  setMultiColor(palette, chaseStep);

  chaseStep = Felix8A::wrap(chaseStep + 1, 0, static_cast<int>(palette.size()));
}

void multiColor(const Felix8A::Palette& palette, bool animated, bool chase, bool wasUpdated) {
  if (animated) {
    if (chase) {
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
  if (!wasUpdated) { return; }

  lightString->clear();
  lightString->show();
}
```

### Set & Update Function Switch
```cpp
void updateMode(uint8_t mode, uint8_t color, bool animated, bool chase, bool wasUpdated) {
  switch (mode) {
    case 0: solidColor(colorPalette[color], animated, chase, wasUpdated); break;
    case 1: multiColor(colorPalette, animated, chase, wasUpdated); break;
    case 2: multiColor(Felix8A::ChristmasLights, animated, chase, wasUpdated); break;
    case 3: multiColor(Felix8A::Sunset, animated, chase, wasUpdated); break;
    case 4: multiColor(Felix8A::Forest, animated, chase, wasUpdated); break;
    case 5: multiColor(Felix8A::Ocean, animated, chase, wasUpdated); break;
    case 6: multiColor(Felix8A::Blush, animated, chase, wasUpdated); break;
    case 7: multiColor(customPalette, animated, chase, wasUpdated); break;
    default: lightsOff(wasUpdated); break;
  }
}
```

## Main Arduino Code

```cpp
/***** Arduino Setup *****/
void setup() {
  loadSettings();
  // Initiate Button
  button.begin();
  button.setHoldTime(BUTTON_HOLD_TIME);
  button.setMultiClickTime(BUTTON_MULTI_CLICK_TIME);
  // Hold button during startup to use RGB ordering.
  const uint8_t pixelType = button.isDown() ? (NEO_RGB + NEO_KHZ800) : (NEO_GRB + NEO_KHZ800);
  lightString = new Adafruit_NeoPixel(NUM_LEDS, LED_PIN, pixelType);
  lightString->begin();
  lightString->setBrightness(LED_BRIGHTNESS);
  lightString->show();
  // Initialize random number generator.
  randomSeed(analogRead(A1));
}

/***** Arduino Loop *****/
void loop() {
  button.update();

  if (button.wasClicked()) {
    nextMode();
    eventActivated = true;
  }

  if (button.wasDoubleClicked()) {
    previousMode();
    eventActivated = true;
  }

  if (button.wasTripleClicked()) {
    isAnimated = !isAnimated;
    eventActivated = true;
  }

  if (button.wasQuadrupleClicked()) {
    chaseAnimation = !chaseAnimation;
    eventActivated = true;
  }

  if (button.wasHeld()) {
    nextColor();
    eventActivated = true;
  }

  updateMode(currentMode, currentColor, isAnimated, chaseAnimation, eventActivated);
  eventActivated = false;
}
```

### Alternate Loop Code (Using Event Polling)
```cpp
/***** Arduino Loop *****/
void loop() {
  button.update();

  Felix8A::Button::Event event;

  while ((event = button.poll()) != Felix8A::Button::Event::None) {
    switch (event) {
      case Felix8A::Button::Event::Click: nextMode(); break;
      case Felix8A::Button::Event::DoubleClick: previousMode(); break;
      case Felix8A::Button::Event::TripleClick: isAnimated = !isAnimated; break;
      case Felix8A::Button::Event::QuadrupleClick: chaseAnimation = !chaseAnimation; break;
      case Felix8A::Button::Event::Hold: nextColor(); break;
      default: break;
    }

    eventActivated = true;
  }

  updateMode(currentMode, currentColor, isAnimated, chaseAnimation, eventActivated);

  eventActivated = false;
}

```

#### My 100 Column Helper

```
/********1*********2*********3*********4*********5*********6*********7*********8*********9*********/
```
