//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef STM32F4_GPIO_PORT_H_
#define STM32F4_GPIO_PORT_H_

// ----------------------------------------------------------------------------

#include "stm32f4xx.h"
#include "stm32f4/gpio.h"

#include "stm32f4/GpioPowerPolicy.h"

// ----------------------------------------------------------------------------

namespace stm32f4
{
  // --------------------------------------------------------------------------

  class GpioPortImplementation
  {
  public:

    using value_t = gpio::reg16_t;
    using reg32_t = gpio::reg32_t;
    using bitsMask_t = gpio::bitsMask_t;
    using bitNumber_t = gpio::portBitNumber_t;
    using portNumber_t = gpio::portNumber_t;
    using index_t = gpio::index_t;

  protected:

    GpioPortImplementation() = default;

    /// \details
    /// Use read/modify/write to change the 2 configuration bits.
    /// \warning Non atomic, the caller must use critical sections.
    inline static void
    __attribute__((always_inline))
    configureMode(GPIO_TypeDef* address, reg32_t mask, reg32_t value)
    {
      address->MODER = ((address->MODER & (~mask)) | (value & mask));
    }

    inline static reg32_t
    __attribute__((always_inline))
    retrieveMode(GPIO_TypeDef* address, reg32_t mask)
    {
      return (address->MODER & mask);
    }

    inline static void
    __attribute__((always_inline))
    configureOutputType(GPIO_TypeDef* address, reg32_t mask, reg32_t value)
    {
      address->OTYPER = ((address->OTYPER & (~mask)) | (value & mask));
    }

    inline static reg32_t
    __attribute__((always_inline))
    retrieveOutputType(GPIO_TypeDef* address, reg32_t mask)
    {
      return (address->OTYPER & mask);
    }

    inline static void
    __attribute__((always_inline))
    configureOutputSpeed(GPIO_TypeDef* address, reg32_t mask, reg32_t value)
    {
      address->OSPEEDR = ((address->OSPEEDR & (~mask)) | (value & mask));
    }

    inline static reg32_t
    __attribute__((always_inline))
    retrieveOutputSpeed(GPIO_TypeDef* address, reg32_t mask)
    {
      return (address->OSPEEDR & mask);
    }

    inline static void
    __attribute__((always_inline))
    configureResistors(GPIO_TypeDef* address, reg32_t mask, reg32_t value)
    {
      address->PUPDR = ((address->PUPDR & (~mask)) | (value & mask));
    }

    inline static reg32_t
    __attribute__((always_inline))
    retrieveResistors(GPIO_TypeDef* address, reg32_t mask)
    {
      return (address->PUPDR & mask);
    }

    inline static void
    __attribute__((always_inline))
    configureAlternateFunction(GPIO_TypeDef* address, index_t index,
        reg32_t mask, reg32_t value)
    {
      address->AFR[index] = ((address->AFR[index] & (~mask)) | (value & mask));
    }

    inline static reg32_t
    __attribute__((always_inline))
    retrieveAlternateFunction(GPIO_TypeDef* address, index_t index,
        reg32_t mask)
    {
      return (address->AFR[index] & mask);
    }

    inline static void
    __attribute__((always_inline))
    setHigh(GPIO_TypeDef* address, bitsMask_t mask)
    {
      address->BSRRL = mask;
    }

    inline static void
    __attribute__((always_inline))
    setLow(GPIO_TypeDef* address, bitsMask_t mask)
    {
      address->BSRRH = mask;
    }

    inline static void
    __attribute__((always_inline))
    toggle(GPIO_TypeDef* address, bitsMask_t mask)
    {
      address->ODR ^= mask;
    }

    inline static value_t
    __attribute__((always_inline))
    readInput(GPIO_TypeDef* address)
    {
      return static_cast<value_t>(address->IDR);
    }

    inline static value_t
    __attribute__((always_inline))
    readOutput(GPIO_TypeDef* address)
    {
      return static_cast<value_t>(address->ODR);
    }

    inline static void
    __attribute__((always_inline))
    writeOutput(GPIO_TypeDef* address, value_t value)
    {
      address->ODR = value;
    }
  };

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  template< //
      template<typename > class PowerPolicy_T = TAllocatedGpioPortPowerDownPolicy //
  >
    class TAllocatedGpioPort : public GpioPortImplementation, //
        public PowerPolicy_T<TAllocatedGpioPort<PowerPolicy_T>>
    {
      using portNumber_t = GpioPortImplementation::portNumber_t;
      using Implementation = GpioPortImplementation;
      using PowerPolicy = PowerPolicy_T<TAllocatedGpioPort<PowerPolicy_T>>;

    public:

      TAllocatedGpioPort(gpio::PortId portId) :
          address(
              reinterpret_cast<GPIO_TypeDef*>(GPIOA_BASE
                  + (static_cast<gpio::portNumber_t>(portId))
                      * (GPIOB_BASE - GPIOA_BASE))), //
          portNumber(static_cast<gpio::portNumber_t>(portId))
      {
      }

      inline gpio::portNumber_t
      __attribute__((always_inline))
      getPortNumber(void)
      {
        return this->portNumber;
      }

      inline gpio::PortId
      __attribute__((always_inline))
      getPortId(void)
      {
        return static_cast<gpio::PortId>(this->portNumber);
      }

      inline GPIO_TypeDef*
      __attribute__((always_inline))
      getAddress(void) const
      {
        return this->address;
      }

      inline void
      powerUp(void)
      {
        PowerPolicy::powerUp(this);
      }

      inline void
      powerDown(void)
      {
        PowerPolicy::powerDown(this);
      }

      inline void
      configureMode(reg32_t mask, reg32_t value) const
      {
        Implementation::configureMode(this->address, mask, value);
      }

      inline reg32_t
      retrieveMode(reg32_t mask) const
      {
        return Implementation::retrieveMode(this->address, mask);
      }

      inline void
      configureOutputType(reg32_t mask, reg32_t value) const
      {
        Implementation::configureOutputType(this->address, mask, value);
      }

      inline reg32_t
      retrieveOutputType(reg32_t mask) const
      {
        return Implementation::retrieveOutputType(this->address, mask);
      }

      inline void
      configureOutputSpeed(reg32_t mask, reg32_t value) const
      {
        Implementation::configureOutputSpeed(this->address, mask, value);
      }

      inline reg32_t
      retrieveOutputSpeed(reg32_t mask) const
      {
        return Implementation::retrieveOutputSpeed(this->address, mask);
      }

      inline void
      configureResistors(reg32_t mask, reg32_t value) const
      {
        Implementation::configureResistors(this->address, mask, value);
      }

      inline reg32_t
      retrieveResistors(reg32_t mask) const
      {
        return Implementation::retrieveResistors(this->address, mask);
      }

      inline void
      configureAlternateFunction(index_t index, reg32_t mask,
          reg32_t value) const
      {
        Implementation::configureAlternateFunction(this->address, index, mask,
            value);
      }

      inline reg32_t
      retrieveAlternateFunction(index_t index, reg32_t mask) const
      {
        return Implementation::retrieveAlternateFunction(this->address, index,
            mask);
      }

      inline void
      setHigh(bitsMask_t mask) const
      {
        Implementation::setHigh(this->address, mask);
      }

      inline void
      setLow(bitsMask_t mask) const
      {
        Implementation::setLow(this->address, mask);
      }

      inline void
      toggle(bitsMask_t mask) const
      {
        Implementation::toggle(this->address, mask);
      }

      inline value_t
      readInput(void) const
      {
        return Implementation::readInput(this->address);
      }

      inline value_t
      readOutput(void) const
      {
        return Implementation::readOutput(this->address);
      }

      inline void
      writeOutput(value_t value) const
      {
        Implementation::writeOutput(this->address, value);
      }

    private:

      // Allocated members
      GPIO_TypeDef* const address;
      portNumber_t const portNumber;

    };

#pragma GCC diagnostic pop


  // --------------------------------------------------------------------------

  template<
      gpio::PortId portId_T, //
      template<typename > class PowerPolicy_T = TConstantGpioPortNoPowerDownPolicy //
  >
    class TConstantGpioPort : public GpioPortImplementation, //
        public PowerPolicy_T<TConstantGpioPort<portId_T, PowerPolicy_T>>
    {
      using Implementation = GpioPortImplementation;
      using PowerPolicy = PowerPolicy_T<TConstantGpioPort<portId_T, PowerPolicy_T>>;

      // Validate template constant parameters
      static_assert(portId_T <= gpio::PortId::MAX, "Port number too high");

    public:

      TConstantGpioPort() = default;

      inline static gpio::portNumber_t
      __attribute__((always_inline))
      getPortNumber(void)
      {
        return PORT_NUMBER;
      }

      inline static gpio::PortId
      __attribute__((always_inline))
      getPortId(void)
      {
        return PORT_ID;
      }

      inline static GPIO_TypeDef*
      __attribute__((always_inline))
      getAddress(void)
      {
        return PORT_ADDRESS;
      }

      inline static void
      __attribute__((always_inline))
      powerUp(void)
      {
        PowerPolicy::powerUp(nullptr);
      }

      inline static void
      __attribute__((always_inline))
      powerDown(void)
      {
        PowerPolicy::powerDown(nullptr);
      }

      inline static void
      __attribute__((always_inline))
      configureMode(reg32_t mask, reg32_t value)
      {
        Implementation::configureMode(PORT_ADDRESS, mask, value);
      }

      inline static reg32_t
      __attribute__((always_inline))
      retrieveMode(reg32_t mask)
      {
        return Implementation::retrieveMode(PORT_ADDRESS, mask);
      }

      inline static void
      __attribute__((always_inline))
      configureOutputType(reg32_t mask, reg32_t value)
      {
        Implementation::configureOutputType(PORT_ADDRESS, mask, value);
      }

      inline static reg32_t
      __attribute__((always_inline))
      retrieveOutputType(reg32_t mask)
      {
        return Implementation::retrieveOutputType(PORT_ADDRESS, mask);
      }

      inline static void
      __attribute__((always_inline))
      configureOutputSpeed(reg32_t mask, reg32_t value)
      {
        Implementation::configureOutputSpeed(PORT_ADDRESS, mask, value);
      }

      inline static reg32_t
      __attribute__((always_inline))
      retrieveOutputSpeed(reg32_t mask)
      {
        return Implementation::retrieveOutputSpeed(PORT_ADDRESS, mask);
      }

      inline static void
      __attribute__((always_inline))
      configureResistors(reg32_t mask, reg32_t value)
      {
        Implementation::configureResistors(PORT_ADDRESS, mask, value);
      }

      inline static reg32_t
      __attribute__((always_inline))
      retrieveResistors(reg32_t mask)
      {
        return Implementation::retrieveResistors(PORT_ADDRESS, mask);
      }

      inline static void
      __attribute__((always_inline))
      configureAlternateFunction(index_t index, reg32_t mask, reg32_t value)
      {
        Implementation::configureAlternateFunction(PORT_ADDRESS, index, mask,
            value);
      }

      inline static reg32_t
      __attribute__((always_inline))
      retrieveAlternateFunction(index_t index, reg32_t mask)
      {
        return Implementation::retrieveAlternateFunction(PORT_ADDRESS, index,
            mask);
      }

      inline static void
      __attribute__((always_inline))
      setHigh(bitsMask_t mask)
      {
        Implementation::setHigh(PORT_ADDRESS, mask);
      }

      inline static void
      __attribute__((always_inline))
      setLow(bitsMask_t mask)
      {
        Implementation::setLow(PORT_ADDRESS, mask);
      }

      inline static void
      __attribute__((always_inline))
      toggle(bitsMask_t mask)
      {
        Implementation::toggle(PORT_ADDRESS, mask);
      }

      inline static value_t
      __attribute__((always_inline))
      readInput(void)
      {
        return Implementation::readInput(PORT_ADDRESS);
      }

      inline static value_t
      __attribute__((always_inline))
      readOutput(void)
      {
        return Implementation::readOutput(PORT_ADDRESS);
      }

      inline static void
      __attribute__((always_inline))
      writeOutput(value_t mask)
      {
        Implementation::writeOutput(PORT_ADDRESS, mask);
      }

    private:

      // Constant members
      static constexpr gpio::PortId PORT_ID = portId_T; //
      static constexpr gpio::portNumber_t PORT_NUMBER =
          static_cast<gpio::portNumber_t>(portId_T);
      static constexpr GPIO_TypeDef* PORT_ADDRESS =
          reinterpret_cast<GPIO_TypeDef*>(GPIOA_BASE
              + PORT_NUMBER * (GPIOB_BASE - GPIOA_BASE));
    };

// ----------------------------------------------------------------------------

}//namespace stm32f4

// ----------------------------------------------------------------------------

#endif // STM32F4_GPIO_PORT_H_
