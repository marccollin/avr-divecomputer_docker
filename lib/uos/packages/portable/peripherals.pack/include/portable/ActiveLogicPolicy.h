//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef PORTABLE_ACTIVE_LOGIC_POLICY_H
#define PORTABLE_ACTIVE_LOGIC_POLICY_H

// ----------------------------------------------------------------------------

template<typename GpioPin_T>
  class TActiveLowPolicy
  {
    using GpioPin = GpioPin_T;

  public:

    static constexpr bool isActiveLow = true;
    static constexpr bool isActiveHigh = false;

    inline static void
    __attribute__((always_inline))
    makeActive(GpioPin& gpioPin)
    {
      gpioPin.setLow();
    }

    inline static void
    __attribute__((always_inline))
    makeInactive(GpioPin& gpioPin)
    {
      gpioPin.setHigh();
    }

    inline static bool
    __attribute__((always_inline))
    isActive(GpioPin& gpioPin)
    {
      return gpioPin.isOutputLow();
    }

  };

// ----------------------------------------------------------------------------

template<typename GpioPin_T>
  class TActiveHighPolicy
  {
    using GpioPin = GpioPin_T;

  public:

    static constexpr bool isActiveLow = false;
    static constexpr bool isActiveHigh = true;

    inline static void
    __attribute__((always_inline))
    makeActive(GpioPin& gpioPin)
    {
      gpioPin.setHigh();
    }

    inline static void
    __attribute__((always_inline))
    makeInactive(GpioPin& gpioPin)
    {
      gpioPin.setLow();
    }

    inline static bool
    __attribute__((always_inline))
    isActive(GpioPin& gpioPin)
    {
      return gpioPin.isOutputHigh();
    }

  };

// ----------------------------------------------------------------------------

#endif // PORTABLE_ACTIVE_LOGIC_POLICY_H
