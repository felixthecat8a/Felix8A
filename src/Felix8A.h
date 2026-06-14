#ifndef FELIX8A_H
#define FELIX8A_H

#include <Arduino.h>

#include "util/Color32.h"
#include "util/Palette.h"
#include "util/Math.h"
#include "util/Time.h"

#define FELIX8A_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

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
      FELIX8A_ARRAY_SIZE(ChristmasTreeColorSet)
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
      FELIX8A_ARRAY_SIZE(SunsetColorSet)
    );

    static const uint32_t ForestColorSet[] = {
      Color::LIME,         Color::CHARTREUSE, Color::HARLEQUIN,
      Color::GREEN,        Color::AO,         Color::MINT,
      Color::SPRING_GREEN, Color::TURQUOISE,  Color::TEAL,
      Color::HONEYDEW,     Color::IVORY,      Color::OLIVE,
    };

    static const Palette Forest(
      ForestColorSet,
      FELIX8A_ARRAY_SIZE(ForestColorSet)
    );

    static const uint32_t OceanColorSet[] = {
      Color::CYAN,       Color::SKY_BLUE,   Color::AZURE,
      Color::SAPPHIRE,   Color::BLUE,       Color::NAVY,
      Color::COOL_WHITE, Color::AQUAMARINE, Color::SEA_GREEN,
    };

    static const Palette Ocean(
      OceanColorSet,
      FELIX8A_ARRAY_SIZE(OceanColorSet)
    );

    static const uint32_t BlushColorSet[] = {
      Color::INDIGO,  Color::VIOLET,    Color::E_PURPLE,
      Color::MAGENTA, Color::HOT_PINK,  Color::ROSE,
      Color::PURPLE,  Color::CRIMSON,   Color::LILAC,
      // Color::LAVENDER, // lighter
    };

    static const Palette Blush(
      BlushColorSet,
      FELIX8A_ARRAY_SIZE(BlushColorSet)
    );

    inline const uint32_t FullColorSet[] = {
      Color::RED,     Color::VERMILION, Color::ORANGE,        Color::AMBER,
      Color::YELLOW,  Color::LIME,      Color::CHARTREUSE,    Color::HARLEQUIN,
      Color::GREEN,   Color::MINT,      Color::SPRING_GREEN,  Color::TURQUOISE,
      Color::CYAN,    Color::SKY_BLUE,  Color::AZURE,         Color::SAPPHIRE,
      Color::BLUE,    Color::INDIGO,    Color::VIOLET,        Color::E_PURPLE,
      Color::MAGENTA, Color::HOT_PINK,  Color::ROSE,          Color::CRIMSON,
    };

    static const Palette FullSpectrum(
      FullColorSet,
      FELIX8A_ARRAY_SIZE(FullColorSet)
    );

  } // namespace Palettes

} // namespace Felix8A

#endif // FELIX8A_H
