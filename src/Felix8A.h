#ifndef FELIX8A_H
#define FELIX8A_H

#include <Arduino.h>

#include "util/Color32.h"
#include "util/Palette.h"
#include "util/Math.h"
#include "util/Time.h"

#define FELIX8A_COLOR_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

namespace Felix8A {
  using Color = Color32;

  namespace Palettes {

    static const uint32_t ChristmasTreeColorSet[] = {
      Color::RED,
      Color::ORANGE,
      Color::GREEN,
      Color::BLUE,
      Color::DIM_WHITE,
    };

    static const Palette ChristmasTree(
      ChristmasTreeColorSet,
      FELIX8A_COLOR_ARRAY_SIZE(ChristmasTreeColorSet)
    );

    static const uint32_t SunsetColorSet[] = {
      Color::RED,
      Color::VERMILION,
      Color::ORANGE,
      Color::AMBER,
      Color::YELLOW,
      Color::WARM_WHITE,
    };

    static const Palette Sunset(
      SunsetColorSet,
      FELIX8A_COLOR_ARRAY_SIZE(SunsetColorSet)
    );

    static const uint32_t ForestColorSet[] = {
      Color::LIME,
      Color::CHARTREUSE,
      Color::HARLEQUIN,
      Color::GREEN,
      Color::MINT,
      Color::SPRING_GREEN,
      Color::TURQUOISE,
    };

    static const Palette Forest(
      ForestColorSet,
      FELIX8A_COLOR_ARRAY_SIZE(ForestColorSet)
    );

    static const uint32_t OceanColorSet[] = {
      Color::CYAN,
      Color::SKY_BLUE,
      Color::AZURE,
      Color::SAPPHIRE,
      Color::BLUE,
      Color::COOL_WHITE,
    };

    static const Palette Ocean(
      OceanColorSet,
      FELIX8A_COLOR_ARRAY_SIZE(OceanColorSet)
    );

    static const uint32_t BlushColorSet[] = {
      Color::INDIGO,
      Color::VIOLET,
      Color::PURPLE,
      Color::MAGENTA,
      Color::HOT_PINK,
      Color::ROSE,
      Color::CRIMSON,
    };

    static const Palette Blush(
      BlushColorSet,
      FELIX8A_COLOR_ARRAY_SIZE(BlushColorSet)
    );

    inline const uint32_t FullColorSet[] = {
      Color::RED,
      Color::VERMILION,
      Color::ORANGE,
      Color::AMBER,
      Color::YELLOW,
      Color::LIME,
      Color::CHARTREUSE,
      Color::HARLEQUIN,
      Color::GREEN,
      Color::MINT,
      Color::SPRING_GREEN,
      Color::TURQUOISE,
      Color::CYAN,
      Color::SKY_BLUE,
      Color::AZURE,
      Color::SAPPHIRE,
      Color::BLUE,
      Color::INDIGO,
      Color::VIOLET,
      Color::PURPLE,
      Color::MAGENTA,
      Color::HOT_PINK,
      Color::ROSE,
      Color::CRIMSON,
    };

    static const Palette FullSpectrum(
      FullColorSet,
      FELIX8A_COLOR_ARRAY_SIZE(FullColorSet)
    );

  } // namespace Palettes

} // namespace Felix8A

#endif // FELIX8A_H
