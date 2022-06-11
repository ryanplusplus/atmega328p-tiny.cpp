/*!
 * @file
 * @brief
 */

#ifndef Heartbeat_hpp
#define Heartbeat_hpp

#include "tiny/Timer.h"

class Heartbeat {
 public:
  Heartbeat(tiny::TimerGroup& timer_group)
    : timer()
  {
    DDRB |= _BV(5);

    timer_group.start_periodic(
      this->timer, period, +[](void*) {
        PINB |= _BV(5);
      });
  }

 private:
  static constexpr tiny::TimerTicks period = 500;
  tiny::Timer timer;
};

#endif
