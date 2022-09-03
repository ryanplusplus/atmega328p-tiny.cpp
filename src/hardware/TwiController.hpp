/*!
 * @file
 * @brief Standard mode (100 KHz) @ 8 MHz CPU clock
 */

#ifndef TwiController_hpp
#define TwiController_hpp

#include "tiny/hal/II2c.hpp"

class TwiController {
  static_assert(F_CPU == 8000000, "8 MHz clock is required");

 public:
  static tiny::II2c& get_instance();
};

#endif
