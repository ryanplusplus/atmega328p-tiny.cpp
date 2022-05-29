/*!
 * @file
 * @brief
 */

#include "Clock.hpp"
#include "SystemTick.hpp"
#include "Watchdog.hpp"
#include "Heartbeat.hpp"
#include "Interrupts.hpp"
#include "tiny/Timer.h"

using namespace tiny;

int main(void)
{
  Interrupts::disable();

  Clock::init();
  TimerGroup timer_group{ SystemTick::get_instance() };
  Watchdog watchdog{ timer_group };
  Heartbeat heartbeat{ timer_group };

  Interrupts::enable();

  while(1) {
    if(timer_group.run()) {
      Interrupts::wait_for_interrupt();
    }
  }
}
