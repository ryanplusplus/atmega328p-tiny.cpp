/*!
 * @file
 * @brief
 */

#include "Clock.hpp"
#include "SystemTick.hpp"
#include "Watchdog.hpp"
#include "Heartbeat.hpp"
#include "Interrupts.hpp"
#include "Neopixel.hpp"
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

  Neopixel::Neopixel<Neopixel::ByteOrder::Grb, Neopixel::Port::B, 0> neopixel{};
  Neopixel::Color<Neopixel::ByteOrder::Grb> color[]{
    { 0, 9, 2 },
    { 0, 8, 3 },
    { 0, 7, 4 },
    { 0, 6, 5 },
    { 0, 5, 6 },
    { 0, 4, 7 },
    { 0, 3, 8 },
    { 0, 2, 9 },
  };
  neopixel.write(color, 8);

  while(1) {
    if(timer_group.run()) {
      Interrupts::wait_for_interrupt();
    }
  }
}
