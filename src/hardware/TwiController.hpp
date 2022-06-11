/*!
 * @file
 * @brief Standard mode (100 KHz) @ 8 MHz CPU clock
 */

#ifndef TwiController_h
#define TwiController_h

#include "tiny/hal/II2c.hpp"

class TwiController {
  static_assert(F_CPU == 8000000, "8 MHz clock is required");

 public:
  static auto get_instance() -> tiny::II2c&;
};

#endif
