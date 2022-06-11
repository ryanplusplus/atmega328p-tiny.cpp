/*!
 * @file
 * @brief
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Usart0.hpp"
#include "tiny/SingleSubscriberEvent.hpp"

static class : public tiny::IUart {
 public:
  auto init(bool u2x, uint16_t brr) -> void
  {
    // Enable USART0 clock
    PRR &= ~_BV(PRUSART0);

    UCSR0A = _BV(u2x);

    // Enable RX, TX, and corresponding interrupts
    UCSR0B = _BV(RXCIE0) | _BV(TXCIE0) | _BV(RXEN0) | _BV(TXEN0);

    // Asynchronous mode, no parity, 1 stop bit, 8 data bits
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

    // Set baud
    UBRR0H = (brr >> 8);
    UBRR0L = (brr & 0xFF);
  }

  auto send(uint8_t byte) -> void override
  {
    UDR0 = byte;
  }

  auto on_send_complete() -> tiny::IEvent<>& override
  {
    return send_complete;
  }

  auto on_receive() -> tiny::IEvent<uint8_t>& override
  {
    return receive;
  }

 public:
  tiny::SingleSubscriberEvent<> send_complete;
  tiny::SingleSubscriberEvent<uint8_t> receive;
} instance;

ISR(USART_RX_vect)
{
  instance.receive.publish(UDR0);
}

ISR(USART_TX_vect)
{
  instance.send_complete.publish();
}

auto Usart0::get_instance(bool u2x, uint16_t baud) -> tiny::IUart&
{
  instance.init(u2x, baud);
  return instance;
}
