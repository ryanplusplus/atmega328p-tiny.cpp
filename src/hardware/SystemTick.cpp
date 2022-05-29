/*!
 * @file
 * @brief
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "SystemTick.hpp"

using namespace tiny;

static volatile ITimeSource::TickCount current_ticks;

ISR(TIMER0_COMPA_vect)
{
  current_ticks++;
}

static class : public ITimeSource {
 public:
  virtual auto ticks() -> TickCount
  {
    TickCount previous;
    TickCount current = current_ticks;

    do {
      previous = current;
      current = current_ticks;
    } while(current != previous);

    return current;
  }
} instance;

auto SystemTick::get_instance() -> ITimeSource&
{
  // Enable TIMER 0 clock
  PRR &= ~_BV(PRTIM0);

  // Clear timer on compare match
  TCCR0A = _BV(WGM01);

  // Prescalar 64
  // Count 125
  // 8 MHz / 64 / 125 == 1000 Hz
  TCCR0B = _BV(CS01) | _BV(CS00);
  OCR0A = 125;

  // Enable compare match A interrupt
  TIMSK0 = _BV(OCIE0A);

  return instance;
}
