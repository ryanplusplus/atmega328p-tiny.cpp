/*!
 * @file
 * @brief
 */

#ifndef SystemTick_hpp
#define SystemTick_hpp

#include "tiny/ITimeSource.h"

class SystemTick {
 public:
  static auto get_instance() -> tiny::ITimeSource&;
};

#endif
