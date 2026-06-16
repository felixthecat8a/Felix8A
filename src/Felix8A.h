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
      Color::MAROON,        Color::RED,         Color::VERMILION,
      Color::OLIVE,         Color::ORANGE,      Color::AMBER,
      Color::YELLOW,        Color::MOCHA,       Color::CORAL,
    };

    static const uint32_t ForestSet[] = {
      Color::LIME,          Color::CHARTREUSE,  Color::HARLEQUIN,
      Color::EMERALD,       Color::GREEN,       Color::MINT,
      Color::SPRING_GREEN,  Color::TURQUOISE,   Color::HONEYDEW,
    };

    static const uint32_t OceanSet[] = {
      Color::TEAL,          Color::CYAN,        Color::SKY_BLUE,
      Color::AZURE,         Color::SAPPHIRE,    Color::NAVY,
      Color::BLUE,          Color::LAVENDER,    Color::CELESTE,
    };

    static const uint32_t BlushSet[] = {
      Color::INDIGO,        Color::VIOLET,      Color::ELECTRIC_PURPLE,
      Color::PURPLE,        Color::MAGENTA,     Color::HOT_PINK,
      Color::ROSE,          Color::CRIMSON,     Color::LILAC,
    };

    static const uint32_t Spectrum36[] = {
      Color::CORAL,     Color::MAROON,  Color::RED,     Color::VERMILION, Color::ORANGE,        Color::AMBER,
      Color::MOCHA,     Color::OLIVE,   Color::YELLOW,  Color::LIME,      Color::CHARTREUSE,    Color::HARLEQUIN,
      Color::HONEYDEW,  Color::EMERALD, Color::GREEN,   Color::MINT,      Color::SPRING_GREEN,  Color::TURQUOISE,
      Color::CELESTE,   Color::TEAL,    Color::CYAN,    Color::SKY_BLUE,  Color::AZURE,         Color::SAPPHIRE,
      Color::LAVENDER,  Color::NAVY,    Color::BLUE,    Color::INDIGO,    Color::VIOLET,        Color::ELECTRIC_PURPLE,
      Color::LILAC,     Color::PURPLE,  Color::MAGENTA, Color::HOT_PINK,  Color::ROSE,          Color::CRIMSON,
    };

    static const uint32_t Spectrum24[] = {
      Color::RED,       Color::VERMILION,   Color::ORANGE,        Color::AMBER,
      Color::YELLOW,    Color::LIME,        Color::CHARTREUSE,    Color::HARLEQUIN,
      Color::GREEN,     Color::MINT,        Color::SPRING_GREEN,  Color::TURQUOISE,
      Color::CYAN,      Color::SKY_BLUE,    Color::AZURE,         Color::SAPPHIRE,
      Color::BLUE,      Color::INDIGO,      Color::VIOLET,        Color::ELECTRIC_PURPLE,
      Color::MAGENTA,   Color::HOT_PINK,    Color::ROSE,          Color::CRIMSON,
    };

    static const uint32_t Spectrum12[] = {
      Color::RED,    Color::ORANGE,       Color::YELLOW,   Color::CHARTREUSE,
      Color::GREEN,  Color::SPRING_GREEN, Color::CYAN,     Color::AZURE,
      Color::BLUE,   Color::VIOLET,       Color::MAGENTA,  Color::ROSE,
    };

    static const uint32_t Spectrum6[] = {
      Color::RED,     Color::YELLOW,    Color::GREEN,
      Color::CYAN,    Color::BLUE,      Color::MAGENTA
    };

    static const uint32_t Spectrum3[] = { Color::RED, Color::GREEN, Color::BLUE };

  } // namespace Sets

  static const Palette ChristmasTree(
    Sets::ChristmasTreeSet,
    FELIX8A_ARRAY_SIZE(Sets::ChristmasTreeSet)
  );

  static const Palette Sunset(Sets::SunsetSet, FELIX8A_ARRAY_SIZE(Sets::SunsetSet));
  static const Palette Forest(Sets::ForestSet, FELIX8A_ARRAY_SIZE(Sets::ForestSet));
  static const Palette Ocean(Sets::OceanSet, FELIX8A_ARRAY_SIZE(Sets::OceanSet));
  static const Palette Blush(Sets::BlushSet, FELIX8A_ARRAY_SIZE(Sets::BlushSet));

  static const Palette Palette36(Sets::Spectrum36, FELIX8A_ARRAY_SIZE(Sets::Spectrum36));
  static const Palette Palette24(Sets::Spectrum24, FELIX8A_ARRAY_SIZE(Sets::Spectrum24));
  static const Palette Palette12(Sets::Spectrum12, FELIX8A_ARRAY_SIZE(Sets::Spectrum12));
  static const Palette Palette6(Sets::Spectrum6, FELIX8A_ARRAY_SIZE(Sets::Spectrum6));
  static const Palette Palette3(Sets::Spectrum3, FELIX8A_ARRAY_SIZE(Sets::Spectrum3));

} // namespace Felix8A

#endif // FELIX8A_H
