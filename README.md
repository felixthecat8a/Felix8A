# Felix8A

![GitHub Release](https://img.shields.io/github/v/release/felixthecat8a/Felix8A?color=00878F&logo=arduino&logoColor=00878F)
[![License: MIT](https://img.shields.io/github/license/felixthecat8a/Felix8A)](https://opensource.org/licenses/MIT)
![GitHub repo file or directory count](https://img.shields.io/github/directory-file-count/felixthecat8a/Felix8A)
![GitHub repo size](https://img.shields.io/github/repo-size/felixthecat8a/Felix8A)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/felixthecat8a/Felix8A?color=00599C)
![GitHub top language](https://img.shields.io/github/languages/top/felixthecat8a/Felix8A?color=00599C&logo=cplusplus&logoColor=00599C)

![GitHub Created At](https://img.shields.io/github/created-at/felixthecat8a/Felix8A)
![GitHub Release Date](https://img.shields.io/github/release-date/felixthecat8a/Felix8A)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/felixthecat8a/LCD_BacklightRGB)
![GitHub commits since latest release](https://img.shields.io/github/commits-since/felixthecat8a/Felix8A/latest)
![GitHub last commit](https://img.shields.io/github/last-commit/felixthecat8a/Felix8A)

An Arduino Library toolkit for handling components in my projects.

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
    // loop code
}
```
