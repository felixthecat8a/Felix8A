#ifndef TIME_H
#define TIME_H

#include <Arduino.h>

namespace Felix8A {
  namespace Time {

    static inline bool every(unsigned long interval, unsigned long& last) {
      unsigned long now = millis();
      unsigned long diff = now - last;
      if (diff >= interval) {
        last += interval * (diff / interval);
        return true;
      }
      return false;
    }

    static inline bool after(unsigned long interval, unsigned long start) {
      return millis() - start >= interval;
    }

    static inline void reset(unsigned long& timestamp) {
      timestamp = millis();
    }

  } // namespace Time
} // namespace Felix8A

#endif // TIME_H
