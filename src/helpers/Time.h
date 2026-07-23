#ifndef FELIX8A_TIME_H
#define FELIX8A_TIME_H

#include <Arduino.h>

namespace Felix8A {

  class Time {
  public:
    static inline bool every(unsigned long interval, unsigned long& last) {
      unsigned long now  = millis();
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

    static inline void reset(unsigned long& timestamp) { timestamp = millis(); }

    static inline bool once(unsigned long interval, unsigned long& start) {
      if (start == 0) return false; // not armed

      if (millis() - start >= interval) {
        start = 0; // disarm so it won't trigger again
        return true;
      }
      return false;
    }
  };

} // namespace Felix8A

#endif // FELIX_8A_H
