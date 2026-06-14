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

    static const uint32_t ChristmasTreeSet[] = {
      Color::RED,
      Color::ORANGE,
      Color::GREEN,
      Color::BLUE,
      Color::DIM_WHITE,
    };

    static const Palette ChristmasTree(
      ChristmasTreeSet,
      FELIX8A_ARRAY_SIZE(ChristmasTreeSet)
    );

    static const uint32_t SunsetSet[] = {
      Color::MAROON,    Color::RED,         Color::VERMILION,
      Color::CHOCOLATE, Color::ORANGE,      Color::AMBER,
      Color::YELLOW,    Color::WARM_WHITE,  Color::MOCHA
    };

    static const Palette Sunset(SunsetSet, FELIX8A_ARRAY_SIZE(SunsetSet));

    static const uint32_t ForestSet[] = {
      Color::LIME,         Color::CHARTREUSE, Color::HARLEQUIN,
      Color::GREEN,        Color::AO,         Color::MINT,
      Color::SPRING_GREEN, Color::TURQUOISE,  Color::TEAL,
      Color::HONEYDEW,     Color::IVORY,      Color::OLIVE,
    };

    static const Palette Forest(ForestSet, FELIX8A_ARRAY_SIZE(ForestSet));

    static const uint32_t OceanSet[] = {
      Color::CYAN,       Color::SKY_BLUE,   Color::AZURE,
      Color::SAPPHIRE,   Color::BLUE,       Color::NAVY,
      Color::COOL_WHITE, Color::AQUAMARINE, Color::SEA_GREEN,
    };

    static const Palette Ocean(OceanSet, FELIX8A_ARRAY_SIZE(OceanSet));

    static const uint32_t BlushSet[] = {
      Color::INDIGO,  Color::VIOLET,    Color::E_PURPLE,
      Color::MAGENTA, Color::HOT_PINK,  Color::ROSE,
      Color::PURPLE,  Color::CRIMSON,   Color::LILAC,
      // Color::LAVENDER, // lighter
    };

    static const Palette Blush(BlushSet, FELIX8A_ARRAY_SIZE(BlushSet));

    inline const uint32_t FullSet[] = {
      Color::RED,     Color::VERMILION, Color::ORANGE,        Color::AMBER,
      Color::YELLOW,  Color::LIME,      Color::CHARTREUSE,    Color::HARLEQUIN,
      Color::GREEN,   Color::MINT,      Color::SPRING_GREEN,  Color::TURQUOISE,
      Color::CYAN,    Color::SKY_BLUE,  Color::AZURE,         Color::SAPPHIRE,
      Color::BLUE,    Color::INDIGO,    Color::VIOLET,        Color::E_PURPLE,
      Color::MAGENTA, Color::HOT_PINK,  Color::ROSE,          Color::CRIMSON,
    };

    static const Palette FullSpectrum(FullSet, FELIX8A_ARRAY_SIZE(FullSet));

  } // namespace Palettes

} // namespace Felix8A

#endif // FELIX8A_H
