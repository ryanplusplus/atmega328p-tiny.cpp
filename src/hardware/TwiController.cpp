/*!
 * @file
 * @brief
 */

#include <stddef.h>
#include <avr/io.h>
#include "TwiController.hpp"

using namespace tiny;

enum {
  write_bit = 0,
  read_bit = 1,

  status_start_transmitted = 0x08,
  status_restart_transmitted = 0x10,
  status_write_address_acked = 0x18,
  status_read_address_acked = 0x40,
  status_write_byte_acked = 0x28,
  status_read_byte_acked = 0x50,
  status_read_byte_nacked = 0x58
};

#define assert_status(expected) \
  do {                          \
    if(status() != expected) {  \
      reset();                  \
      return false;             \
    }                           \
  } while(0)

#define assert_status_or(expected, alternative)         \
  do {                                                  \
    uint8_t _status = status();                         \
    if(_status != expected && _status != alternative) { \
      reset();                                          \
      return false;                                     \
    }                                                   \
  } while(0)

static class : public II2c {
 public:
  void init()
  {
    // Enable TWI clock
    PRR &= ~_BV(PRTWI);

    reset();
  }

  static uint8_t status()
  {
    return TWSR & 0xFC;
  }

  static void wait_for_complete()
  {
    loop_until_bit_is_set(TWCR, TWINT);
  }

  static void start()
  {
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA);
    wait_for_complete();
  }

  static void stop()
  {
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
  }

  static uint8_t read_byte_ack()
  {
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
    wait_for_complete();
    return TWDR;
  }

  static uint8_t read_byte_nack()
  {
    TWCR = _BV(TWINT) | _BV(TWEN);
    wait_for_complete();
    return TWDR;
  }

  static void write_byte(uint8_t byte)
  {
    TWDR = byte;
    TWCR = _BV(TWINT) | _BV(TWEN);
    wait_for_complete();
  }

  bool write(
    uint8_t address,
    bool prepare_for_restart,
    const uint8_t* _buffer,
    uint16_t buffer_size) override
  {
    auto buffer = reinterpret_cast<const uint8_t*>(_buffer);

    start();
    assert_status_or(status_start_transmitted, status_restart_transmitted);

    write_byte((address << 1) | write_bit);
    assert_status(status_write_address_acked);

    for(uint16_t i = 0; i < buffer_size; i++) {
      write_byte(buffer[i]);
      assert_status(status_write_byte_acked);
    }

    if(!prepare_for_restart) {
      stop();
    }

    return true;
  }

  bool read(
    uint8_t address,
    bool prepare_for_restart,
    uint8_t* _buffer,
    uint16_t buffer_size) override
  {
    auto buffer = reinterpret_cast<uint8_t*>(_buffer);

    start();
    assert_status_or(status_start_transmitted, status_restart_transmitted);

    write_byte((address << 1) | read_bit);
    assert_status(status_read_address_acked);

    for(uint16_t i = 0; i < buffer_size; i++) {
      if(i + 1 < buffer_size) {
        buffer[i] = read_byte_ack();
        assert_status(status_read_byte_acked);
      }
      else {
        buffer[i] = read_byte_nack();
        assert_status(status_read_byte_nacked);
      }
    }

    if(!prepare_for_restart) {
      stop();
    }

    return true;
  }

  void reset() override
  {
    // Disable peripheral
    TWCR &= ~_BV(TWEN);

    // 8 MHz / (16 + 2 * TWBR=32 * Prescalar=1) => ~100 KHz
    // Prescalar defaults to 1 in TWSR
    TWBR = 32;

    // Enable peripheral
    TWCR |= _BV(TWEN);
  }
} instance;

tiny::II2c& TwiController::get_instance()
{
  instance.init();
  return instance;
}
