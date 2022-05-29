/*!
 * @file
 * @brief
 */

#ifndef Clock_hpp
#define Clock_hpp

#include <avr/io.h>

class Clock {
 public:
  static auto init() -> void
  {
    // Gate peripheral clocks
    PRR = 0;
  }
};

#endif
