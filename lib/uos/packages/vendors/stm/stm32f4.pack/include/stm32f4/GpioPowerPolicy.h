//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef STM32F4_GPIO_POWER_POLICY_H_
#define STM32F4_GPIO_POWER_POLICY_H_

// ----------------------------------------------------------------------------

#include "stm32f4/gpio.h"
#include "stm32f4/Rcc.h"

// ----------------------------------------------------------------------------

// The port power policies are used as base class for ports.
// Not to be used directly.
// On constant port definitions, the pointer is null.

namespace stm32f4
{
  // --------------------------------------------------------------------------

  template<typename GpioPort_T>
    class TConstantGpioPortNoPowerDownPolicy
    {
      using GpioPort = GpioPort_T;

    protected:

      TConstantGpioPortNoPowerDownPolicy() = default;

      inline static void
      __attribute__((always_inline))
      powerUp(GpioPort* gpioPort)
      {
        Rcc::enableGpioPeripheralClock(gpioPort->getPortId());
      }

      inline static void
      __attribute__((always_inline))
      powerDown(GpioPort* gpioPort)
      {
        // No action to power down
      }
    };

  // --------------------------------------------------------------------------

  template<typename GpioPort_T>
    class TAllocatedGpioPortPowerDownPolicy
    {
      using GpioPort = GpioPort_T;

    protected:

      TAllocatedGpioPortPowerDownPolicy()
      {
        count = 0;
      }

      void
      powerUp(GpioPort* gpioPort)
      {
        Rcc::enableGpioPeripheralClock(gpioPort->getPortId());
        ++count;
      }

      void
      powerDown(GpioPort* gpioPort)
      {
        if (count > 0)
          {
            --count;
            if (count > 0)
              {
                // The last one turns off the clock.
                Rcc::disableGpioPeripheralClock(gpioPort->getPortId());
              }
          }
      }

    private:

      // Count how many active
      uint_least16_t count;
    };

// ----------------------------------------------------------------------------
}

// Pin power policies are used as base class for derived classed (like leds).
// Not to be used directly.
// On constant port definitions, the pointer is null.

namespace stm32f4
{
  // --------------------------------------------------------------------------

  template<typename GpioPin_T>
    class TGpioOutputPinPolicy
    {
      using GpioPin = GpioPin_T;

    protected:

      static void
      //__attribute__((always_inline))
      powerUp(GpioPin& gpioPin)
      {
        gpioPin.getPort()->powerUp();

        gpioPin.configureMode(gpio::Mode::Output);
        gpioPin.configureOutputType(gpio::OutputType::PushPull);
        gpioPin.configureOutputSpeed(gpio::OutputSpeed::High_100MHz);
        gpioPin.configureResistors(gpio::Resistors::None);
      }

      static void
      //__attribute__((always_inline))
      powerDown(GpioPin& gpioPin)
      {
        gpioPin.configureMode(gpio::Mode::Input);

        gpioPin.getPort()->powerDown();
      }
    };

// ----------------------------------------------------------------------------

}// namespace stm32f4

// ----------------------------------------------------------------------------

#endif // STM32F4_GPIO_POWER_POLICY_H_

