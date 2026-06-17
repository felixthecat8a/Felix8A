# Felix8A

![GitHub Release](https://img.shields.io/github/v/release/felixthecat8a/Felix8A?color=00878F)
![License: MIT](https://img.shields.io/badge/license-MIT-yellow?link=https://opensource.org/licenses/MIT)
![GitHub repo size](https://img.shields.io/github/repo-size/felixthecat8a/Felix8A)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/felixthecat8a/Felix8A)
![GitHub repo file or directory count](https://img.shields.io/github/directory-file-count/felixthecat8a/Felix8A)
![GitHub top language](https://img.shields.io/github/languages/top/felixthecat8a/FelixTheCatLED?color=%2300599C)

An Arduino Library toolkit for handling 32-bit hexadecimal colors in my projects.

## Installation

### Install via ZIP file

1. Download the *.zip* file of the [latest release](https://github.com/felixthecat8a/Felix8A/releases/latest/).
2. In the **Arduino IDE**, go to ***Sketch*** > ***Include Library*** > ***Add .ZIP Library...***.
3. Select the downloaded *.zip* file.

### Updating the Library

To update the library:
1. Delete the existing ***Felix8A*** folder from your libraries directory: ***Documents*** > ***Arduino*** > ***libraries*** > ***Felix8A***
2. Follow the installation steps above to install the latest version.

---

## Implementation

```cpp
#include <Felix8A.h>

const uint32_t colorArray[] = {
    Felix8A::Color::RED,
    Felix8A::Color::ORANGE,
    Felix8A::Color::GREEN,
    Felix8A::Color::BLUE,
    Felix8A::Color::WHITE
};

const Felix8A::Palette ColorPalette(colorArray);
const int numColors = ColorPalette.size();

void setup() {
    // setup code
}

void loop() {
    // code
}
```

---

## Implementing `Felix8A::Time`

```cpp
#include <Felix8A.h>

unsigned long lastUpdate = 0;
const unsigned long timeInterval = 1000;

void setup() {
  // setup code here
}

void loop() {
  if (Felix8A::Time::every(timeInterval, lastUpdate)) {
    // loop code here
  }

  /* similiar behavior */
  // if (Felix8A::Time::after(timeInterval, lastUpdate)) {
  //   // loop code here
  //   Felix8A::Time::reset(lastUpdate); // reset the timer
  // }
}
```

inside functions

```cpp
void myFunction {
  if (!Felix8A::Time::after(3000, lastUpdate)) {
    return;
  }
  // code
}
```

### `Felix8A::Time` As a One-shot Timer

```cpp
#include <Felix8A.h>

unsigned long startTime = 0;
bool done = false;

void setup() {
  Serial.begin(115200);
  Felix8A::Time::reset(startTime);
}

void loop() {
  if (!done && Felix8A::Time::after(3000, startTime)) {
    done = true;
    Serial.println("3 seconds passed!");
  }
}
```
