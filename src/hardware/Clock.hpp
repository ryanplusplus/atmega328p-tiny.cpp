/*!
 * @file
 * @brief
 */

#ifndef Clock_hpp
#define Clock_hpp

#include <avr/io.h>

class Clock {
 public:
  static void init()
  {
    // Gate peripheral clocks
    PRR = 0;
  }
};

#endif
