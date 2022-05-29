/*!
 * @file
 * @brief
 */

#ifndef Neopixel_hpp
#define Neopixel_hpp

#include <cstdint>
#include "Interrupts.hpp"

namespace Neopixel {
  enum Port {
    B,
    C,
    D
  };

  enum ByteOrder {
    Grb,
    Rgb,
    Rgbw
  };

  template <ByteOrder byte_order>
  struct Color;

  template <>
  struct Color<ByteOrder::Grb> {
    uint8_t g;
    uint8_t r;
    uint8_t b;
  };

  template <>
  struct Color<ByteOrder::Rgb> {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

  template <>
  struct Color<ByteOrder::Rgbw> {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
  };

  template <ByteOrder byte_order, Port port, uint8_t pin>
  class Neopixel {
   public:
    Neopixel()
    {
      _SFR_IO8(ddr_address()) |= _BV(pin);
    }

   public:
    auto write(const Color<byte_order>* data, uint16_t count) -> void
    {
      Interrupts::critical_section([&]() {
        for(uint16_t i = 0; i < count; i++) {
          for(uint8_t j = 0; j < sizeof(data[0]); j++) {
            send_byte(((const uint8_t*)data)[i * sizeof(data[0]) + j]);
          }
        }
      });
    }

    auto write_all(const Color<byte_order>* data, uint16_t count) -> void
    {
      Interrupts::critical_section([&]() {
        for(uint16_t i = 0; i < count; i++) {
          for(uint8_t j = 0; j < sizeof(data[0]); j++) {
            send_byte(((const uint8_t*)data)[j]);
          }
        }
      });
    }

    auto reset() -> void
    {
#if F_CPU == 8000000UL
      // 50 usec == 400 cycles @ 8 MHz
      __asm__ __volatile__(
        "reset_neopixel:\n"
        "	 ldi r16, 100\n" // 1 cycle
        "loop:\n"
        "	 dec r16\n" // 1 cycle
        "	 brne loop\n" // 2 cycles (if it branches)
        :
        :
        : "r16");
#else
#error "Unsupported F_CPU"
#endif
    }

   private:
    static constexpr auto port_address() -> uint16_t const
    {
      switch(port) {
        case Port::B:
          return _SFR_IO_ADDR(PORTB);

        case Port::C:
          return _SFR_IO_ADDR(PORTC);

        default:
        case Port::D:
          return _SFR_IO_ADDR(PORTD);
      }
    }

    static constexpr auto ddr_address() -> uint16_t const
    {
      switch(port) {
        case Port::B:
          return _SFR_IO_ADDR(DDRB);

        case Port::C:
          return _SFR_IO_ADDR(DDRC);

        default:
        case Port::D:
          return _SFR_IO_ADDR(DDRD);
      }
    }

    static auto send_byte(uint8_t byte) -> void
    {
#if F_CPU == 8000000UL
      __asm__ __volatile__(
        "send_byte:\n"
        "  ldi r16,8\n"
        "\n"
        "send_bit:\n"
        "  sbi %[_port], %[_port_bit]\n"
        "  lsl %[byte]\n"
        "  brcs send_1\n"
        "\n"
        "send_0:\n"
        "  cbi %[_port], %[_port_bit]\n"
        "  nop\n"
        "  nop\n"
        "  dec r16\n"
        "  brne send_bit\n"
        "  rjmp done$\n"
        "\n"
        "send_1:\n"
        "  nop\n"
        "  dec r16\n"
        "  cbi %[_port], %[_port_bit]\n"
        "  brne send_bit\n"
        "\n"
        "done:\n"
        :
        : [byte] "r"(byte), [_port] "M"(port_address()), [_port_bit] "M"(pin)
        : "r16", "memory");
#else
#error "Unsupported F_CPU"
#endif
    }
  };
}

#endif
