//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef STM32F4_GPIO_PIN_POWER_POLICY_H_
#define STM32F4_GPIO_PIN_POWER_POLICY_H_

// ----------------------------------------------------------------------------

#include "stm32f4/GpioPin.h"

// ----------------------------------------------------------------------------

namespace stm32f4
{
  // --------------------------------------------------------------------------

  template<typename GpioPin_T>
    class TGpioOutputPinNoPowerDownPolicy
    {
      using GpioPin = GpioPin_T;

    public:
      inline static void
      __attribute__((always_inline))
      powerUp(GpioPin& gpioPin)
      {
        using namespace stm32f4;

        // TODO: enable clock
        gpioPin.configureMode(gpio::Mode::Output);

        gpioPin.configureOutputType(gpio::OutputType::PushPull);
        gpioPin.configureOutputSpeed(gpio::OutputSpeed::High_100MHz);
        gpioPin.configureResistors(gpio::Resistors::None);
      }

      inline static void
      __attribute__((always_inline))
      powerDown(GpioPin& gpioPin)
      {
        gpioPin.configureMode(stm32f4::gpio::Mode::Input);
      }
    };

// ----------------------------------------------------------------------------
}// namespace stm32f4

// ----------------------------------------------------------------------------

#endif // STM32F4_GPIO_PIN_POWER_POLICY_H_

