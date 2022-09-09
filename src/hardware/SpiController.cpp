/*!
 * @file
 * @brief
 */

#include <avr/io.h>
#include "SpiController.hpp"

#define SPI_DDR DDRB
#define SS PINB2
#define MOSI PINB3
#define MISO PINB4
#define SCK PINB5

using namespace tiny;

static class : public ISpi {
 public:
  void init(
    uint8_t cpol,
    uint8_t cpha,
    bool msb_first,
    SpiController::Baud baud)
  {
    // Enable SPI clock
    PRR &= ~_BV(PRSPI);

    // Set SS, MOSI, and SCK as outputs
    // SS must be configured as an output to make sure that
    // the SPI peripheral doesn't switch to slave mode
    SPI_DDR |= _BV(SS) | _BV(MOSI) | _BV(SCK);

    // Enable SPI in master mode, set CPOL and CPHA, set bit
    // order, and set SPR0/1 to (partially) configure SCK
    // frequency
    SPCR = _BV(SPE) | _BV(MSTR) | (cpol << CPOL) | (cpha << CPHA) | (!msb_first << DORD) | (baud & 0x03);

    // Set SPI2X based on the configured baud to finish
    // configuring SCK frequency
    SPSR = baud >> 2;
  }

  void transfer(
    const uint8_t* _write_buffer,
    uint8_t* _read_buffer,
    uint16_t buffer_size) override
  {
    auto write_buffer = reinterpret_cast<const uint8_t*>(_write_buffer);
    auto read_buffer = reinterpret_cast<uint8_t*>(_read_buffer);

    for(uint16_t i = 0; i < buffer_size; i++) {
      SPDR = write_buffer ? write_buffer[i] : 0x00;
      loop_until_bit_is_set(SPSR, SPIF);
      if(read_buffer) {
        read_buffer[i] = SPDR;
      }
    }
  }
} instance;

tiny::ISpi& SpiController::get_instance(
  uint8_t cpol,
  uint8_t cpha,
  bool msb_first,
  Baud baud)
{
  instance.init(cpol, cpha, msb_first, baud);
  return instance;
}
