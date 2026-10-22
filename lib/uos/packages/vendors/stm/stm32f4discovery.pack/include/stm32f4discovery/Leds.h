//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef BOARD_LEDS_H_
#define BOARD_LEDS_H_

#include "portable/Led.h"
#include "portable/ActiveLogicPolicy.h"

#include "stm32f4/GpioPort.h"
#include "stm32f4/GpioPin.h"
#include "stm32f4/GpioPowerPolicy.h"

// ----------------------------------------------------------------------------

// The STM32F4DISCOVERY board has 4 user leds, all active high.
// The numbering is strange, but reflects the STM schematic and the
// position on the board.
//
//      LED4 Green, PD12 (West)
//      LED3 Orange, PD13 (North)
//      LED5 Red, PD14 (East)
//      LED6 Blue, PD15 (South)

namespace
{
  using namespace stm32f4;

  // All leds use the same port, D; this port is defined as a constant static
  // type.
  using GpioPortD = stm32f4::TConstantGpioPort<gpio::PortId::D>;

  // For brevity, the pin definition for port D is aliased.
  template<gpio::PortBitId Pin_T>
    using GpioPinD = stm32f4::TConstantGpioPin<GpioPortD, Pin_T>;

  // Define the types of the 4 pins used by the leds (constant static
  // definitions).
  using GpioPinD12 = GpioPinD<gpio::PortBitId::_12>;
  using GpioPinD13 = GpioPinD<gpio::PortBitId::_13>;
  using GpioPinD14 = GpioPinD<gpio::PortBitId::_14>;
  using GpioPinD15 = GpioPinD<gpio::PortBitId::_15>;

  // For brevity, the led definition using the NoPowerDown and
  // ActiveHigh policies is aliased to a shorter name.
  template<typename GpioPin_T>
    using Led = TConstantLed<GpioPin_T, TGpioOutputPinPolicy, TActiveHighPolicy>;

  // Define the types for all 4 leds (constant static definitions).
  using Led4Green = Led<GpioPinD12>;
  using Led3Orange = Led<GpioPinD13>;
  using Led5Red = Led<GpioPinD14>;
  using Led6Blue = Led<GpioPinD15>;
}

// ----------------------------------------------------------------------------

// The constant leds are in fact static objects, so the usual way of
// accessing them would be with double colon:
//
//    Led4Green::turnOn();
//
// However, for increased portability, the recommended way would be
// with a single dot, like regular objects:
//
//    led4Green.turnOn();
//
// For this to work, a trick is used, we pretend to define
// a led object somewhere, but never do it (the definitions are static
// anyway).

namespace stm32f4discovery
{
  extern Led4Green led4Green;
  extern Led3Orange led3Orange;
  extern Led5Red led5Red;
  extern Led6Blue led6Blue;
}

// ----------------------------------------------------------------------------

#endif // BOARD_LEDS_H_
