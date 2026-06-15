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

  namespace Sets {

    static const uint32_t ChristmasTreeSet[] = {
      Color::RED, Color::ORANGE, Color::GREEN, Color::BLUE, Color::GRAY,
    };

    static const uint32_t SunsetSet[] = {
      Color::MAROON,    Color::RED,         Color::VERMILION,
      Color::CHOCOLATE, Color::ORANGE,      Color::AMBER,
      Color::YELLOW,    Color::WARM_WHITE,  Color::MOCHA
    };

    static const uint32_t ForestSet[] = {
      Color::LIME,         Color::CHARTREUSE, Color::HARLEQUIN,
      Color::GREEN,        Color::AO,         Color::MINT,
      Color::SPRING_GREEN, Color::TURQUOISE,  Color::TEAL,
      Color::HONEYDEW,     Color::IVORY,      Color::OLIVE,
    };

    static const uint32_t OceanSet[] = {
      Color::CYAN,       Color::SKY_BLUE,   Color::AZURE,
      Color::SAPPHIRE,   Color::BLUE,       Color::NAVY,
      Color::COOL_WHITE, Color::AQUAMARINE, Color::SEA_GREEN,
    };

    static const uint32_t BlushSet[] = {
      Color::INDIGO,  Color::VIOLET,    Color::E_PURPLE,
      Color::MAGENTA, Color::HOT_PINK,  Color::ROSE,
      Color::PURPLE,  Color::CRIMSON,   Color::LILAC,
      // Color::LAVENDER, // lighter
    };

  } // namespace Sets

  static const Palette ChristmasTree(
    Sets::ChristmasTreeSet,
    FELIX8A_ARRAY_SIZE(Sets::ChristmasTreeSet)
  );

  static const Palette Sunset(Sets::SunsetSet, FELIX8A_ARRAY_SIZE(Sets::SunsetSet));
  static const Palette Forest(Sets::ForestSet, FELIX8A_ARRAY_SIZE(Sets::ForestSet));
  static const Palette Ocean(Sets::OceanSet, FELIX8A_ARRAY_SIZE(Sets::OceanSet));
  static const Palette Blush(Sets::BlushSet, FELIX8A_ARRAY_SIZE(Sets::BlushSet));

  static const Palette Palette24(Color::Spectrum24, FELIX8A_ARRAY_SIZE(Color::Spectrum24));
  static const Palette Palette12(Color::Spectrum12, FELIX8A_ARRAY_SIZE(Color::Spectrum12));
  static const Palette Palette6(Color::Spectrum6, FELIX8A_ARRAY_SIZE(Color::Spectrum6));
  static const Palette Palette3(Color::Spectrum3, FELIX8A_ARRAY_SIZE(Color::Spectrum3));

} // namespace Felix8A

#endif // FELIX8A_H
