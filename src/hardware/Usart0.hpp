/*!
 * @file
 * @brief
 */

#ifndef Usart0_hpp
#define Usart0_hpp

#include "tiny/hal/IUart.h"

class Usart0 {
  static auto get_instance(uint16_t baud) -> tiny::IUart&;
};

#endif
