/*!
 * @file
 * @brief
 */

#ifndef Interrupts_hpp
#define Interrupts_hpp

#include <avr/interrupt.h>
#include <avr/io.h>
#include <cstdint>

class Interrupts {
 public:
  static void enable()
  {
    sei();
  }

  static void disable()
  {
    cli();
  }

  static void wait_for_interrupt()
  {
    SMCR = _BV(SE);
    __asm__ __volatile__("sleep");
  }

  static uint8_t save()
  {
    uint8_t state = SREG;
    disable();
    return state;
  }

  static void restore(uint8_t state)
  {
    SREG = state;
  }

  template <typename t>
  static void critical_section(t body)
  {
    uint8_t state = Interrupts::save();
    body();
    Interrupts::restore(state);
  }
};

#endif
