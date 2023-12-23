/*!
 * @file
 * @brief
 */

#ifndef SpiController_hpp
#define SpiController_hpp

#include <stdbool.h>
#include <stdint.h>
#include "tiny/hal/ISpi.hpp"

class SpiController {
  static_assert(F_CPU == 8'000'000, "8 MHz clock is required");

 public:
  enum Baud : uint8_t {
    baud_4mhz = 4,
    baud_2mhz = 0,
    baud_1mhz = 5,
    baud_500khz = 1,
    baud_250khz = 6,
    baud_125khz = 2,
    baud_62500hz = 3
  };

  static tiny::ISpi& get_instance(
    uint8_t cpol,
    uint8_t cpha,
    bool msb_first,
    Baud baud);
};

#endif
