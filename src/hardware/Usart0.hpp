/*!
 * @file
 * @brief
 */

#ifndef Usart0_hpp
#define Usart0_hpp

#include "tiny/hal/IUart.hpp"

class Usart0 {
  static tiny::IUart& get_instance(bool u2x, uint16_t brr);
};

#endif
