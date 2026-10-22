//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef STM32F4_GPIO_PIN_H_
#define STM32F4_GPIO_PIN_H_

// ----------------------------------------------------------------------------

#include "stm32f4/gpio.h"

// ----------------------------------------------------------------------------

namespace stm32f4
{
  // --------------------------------------------------------------------------

  template<typename GpioPort_T>
    class TGpioPinImplementation
    {
      using GpioPort = GpioPort_T;

    public:

      using value_t = typename GpioPort_T::value_t;
      using bitMask_t = typename GpioPort_T::bitsMask_t;
      using bitNumber_t = uint_least8_t;

      inline static void
      __attribute__((always_inline))
      configureMode(GpioPort* port, bitNumber_t bitNumber, gpio::Mode value)
      {
        port->configureMode(
            (static_cast<gpio::reg32_t>(gpio::MODE_MASK)) << (bitNumber * 2u),
            (static_cast<gpio::reg32_t>(value)) << (bitNumber * 2u));
      }

      inline static gpio::Mode
      __attribute__((always_inline))
      retrieveMode(GpioPort* port, bitNumber_t bitNumber)
      {
        return static_cast<gpio::Mode>((port->getAddress()->MODER
            >> (bitNumber * 2u)) & gpio::MODE_MASK);
      }

      inline static void
      __attribute__((always_inline))
      configureOutputType(GpioPort* port, bitNumber_t bitNumber,
          gpio::OutputType value)
      {
        port->configureOutputType(
            (static_cast<gpio::reg32_t>(gpio::OUTPUT_TYPE_MASK)) << (bitNumber),
            (static_cast<gpio::reg32_t>(value)) << (bitNumber));
      }

      inline static gpio::OutputType
      __attribute__((always_inline))
      retrieveOutputType(GpioPort* port, bitNumber_t bitNumber)
      {
        return static_cast<gpio::Mode>((port->getAddress()->OTYPER
            >> (bitNumber)) & gpio::OUTPUT_TYPE_MASK);
      }

      inline static void
      __attribute__((always_inline))
      configureOutputSpeed(GpioPort* port, bitNumber_t bitNumber,
          gpio::OutputSpeed value)
      {
        port->configureOutputSpeed(
            (static_cast<gpio::reg32_t>(gpio::OUTPUT_SPEED_MASK))
                << (bitNumber * 2),
            (static_cast<gpio::reg32_t>(value)) << (bitNumber * 2));
      }

      inline static gpio::OutputSpeed
      __attribute__((always_inline))
      retrieveOutputSpeed(GpioPort* port, bitNumber_t bitNumber)
      {
        return static_cast<gpio::OutputSpeed>((port->getAddress()->OSPEEDR
            >> (bitNumber)) & gpio::OUTPUT_SPEED_MASK);
      }

      inline static void
      __attribute__((always_inline))
      configureResistors(GpioPort* port, bitNumber_t bitNumber,
          gpio::Resistors value)
      {
        port->configureResistors(
            (static_cast<gpio::reg32_t>(gpio::RESISTORS_MASK))
                << (bitNumber * 2),
            (static_cast<gpio::reg32_t>(value)) << (bitNumber * 2));
      }

      inline static gpio::Resistors
      __attribute__((always_inline))
      retrieveResistors(GpioPort* port, bitNumber_t bitNumber)
      {
        return static_cast<gpio::Resistors>((port->getAddress()->PUPDR
            >> (bitNumber)) & gpio::RESISTORS_MASK);
      }

      inline static void
      __attribute__((always_inline))
      configureAlternateFunction(GpioPort* port, bitNumber_t bitNumber,
          gpio::AlternateFunction value)
      {
        gpio::index_t const index = (bitNumber / 8);
        bitNumber = (bitNumber % 8);

        port->configureAlternateFunction(index,
            (static_cast<gpio::reg32_t>(gpio::ALTERNATE_FUNCTION_MASK))
                << (bitNumber * 4),
            (static_cast<gpio::reg32_t>(value)) << (bitNumber * 4));
      }

      inline static gpio::AlternateFunction
      __attribute__((always_inline))
      retrieveAlternateFunction(GpioPort* port, bitNumber_t bitNumber)
      {
        gpio::index_t const index = (bitNumber / 8);
        bitNumber = (bitNumber % 8);

        return static_cast<gpio::AlternateFunction>((port->getAddress()->AFR[index]
            >> (bitNumber * 4)) & gpio::ALTERNATE_FUNCTION_MASK);
      }

      inline static void
      __attribute__((always_inline))
      setHigh(GpioPort* port, bitMask_t bitMask)
      {
        port->setHigh(bitMask);
      }

      inline static void
      __attribute__((always_inline))
      setLow(GpioPort* port, bitMask_t bitMask)
      {
        port->setLow(bitMask);
      }

      inline static void
      __attribute__((always_inline))
      toggle(GpioPort* port, bitMask_t bitMask)
      {
        port->toggle(bitMask);
      }

      inline static bool
      __attribute__((always_inline))
      isOutputLow(GpioPort* port, bitMask_t bitMask)
      {
        return ((port->readOutput() & bitMask) == 0);
      }

      inline static bool
      __attribute__((always_inline))
      isOutputHigh(GpioPort* port, bitMask_t bitMask)
      {
        return ((port->readOutput() & bitMask) != 0);
      }

    };

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  template<typename GpioPort_T>
    class TAllocatedGpioPin : public TGpioPinImplementation<GpioPort_T>
    {
      using GpioPort = GpioPort_T;
      using Implementation = TGpioPinImplementation<GpioPort_T>;

    public:

      using bitNumber_t = typename TGpioPinImplementation<GpioPort_T>::bitNumber_t;
      using bitMask_t = typename GpioPort_T::bitsMask_t;

      TAllocatedGpioPin(GpioPort& port_, gpio::PortBitId bitId) :
          port(port_), //
          bitNumber(static_cast<bitNumber_t>(bitId)), //
          bitMask(
              static_cast<bitMask_t>(1u << (static_cast<bitNumber_t>(bitId))))
      {
      }

      inline GpioPort*
      __attribute__((always_inline))
      getPort(void) const
      {
        return &this->port;
      }

      inline gpio::PortBitId
      __attribute__((always_inline))
      getBitId(void) const
      {
        return static_cast<gpio::PortBitId>(this->bitNumber);
      }

      inline bitNumber_t
      __attribute__((always_inline))
      getBitNumber(void) const
      {
        return this->bitNumber;
      }

      inline void
      configureMode(gpio::Mode value) const
      {
        Implementation::configureMode(&this->port, this->bitNumber, value);
      }

      inline gpio::Mode
      retrieveMode(void) const
      {
        return Implementation::configureOutputType(&this->port, this->bitNumber);
      }

      inline void
      configureOutputType(gpio::OutputType value) const
      {
        Implementation::configureOutputType(&this->port, this->bitNumber,
            value);
      }

      inline gpio::OutputType
      retrieveOutputType(void) const
      {
        return Implementation::retrieveOutputType(&this->port, this->bitNumber);
      }

      inline void
      configureOutputSpeed(gpio::OutputSpeed value) const
      {
        Implementation::configureOutputSpeed(&this->port, this->bitNumber,
            value);
      }

      inline gpio::OutputSpeed
      retrieveOutputSpeed(void) const
      {
        return Implementation::retrieveOutputSpeed(&this->port, this->bitNumber);
      }

      inline void
      configureResistors(gpio::Resistors value) const
      {
        Implementation::configureResistors(&this->port, this->bitNumber, value);
      }

      inline gpio::Resistors
      retrieveResistors(void) const
      {
        return Implementation::retrieveResistors(&this->port, this->bitNumber);
      }

      inline void
      configureAlternateFunction(gpio::AlternateFunction value) const
      {
        Implementation::configureAlternateFunction(&this->port, this->bitNumber,
            value);
      }

      inline gpio::AlternateFunction
      retrieveAlternateFunction(void) const
      {
        return Implementation::retrieveAlternateFunction(&this->port,
            this->bitNumber);
      }

      inline void
      setHigh(void) const
      {
        Implementation::setHigh(&this->port, this->bitMask);
      }

      inline void
      setLow(void) const
      {
        Implementation::setLow(&this->port, this->bitMask);
      }

      inline void
      toggle(void) const
      {
        Implementation::toggle(&this->port, this->bitMask);
      }

      inline bool
      isOutputLow(void) const
      {
        return Implementation::isOutputLow(&this->port, this->bitMask);
      }

      inline bool
      isOutputHigh(void) const
      {
        return Implementation::isOutputHigh(&this->port, this->bitMask);
      }

    private:

      GpioPort& port;
      bitNumber_t const bitNumber;
      bitMask_t const bitMask;
    };

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  template<typename GpioPort_T, gpio::PortBitId bitId_T>
    class TConstantGpioPin : public TGpioPinImplementation<GpioPort_T>
    {
      using GpioPort = GpioPort_T;
      using Implementation = TGpioPinImplementation<GpioPort_T>;

      // Validate template constant parameters
      static_assert(bitId_T <= gpio::PortBitId::MAX, "Port bit number too high");

    public:

      using bitNumber_t = typename TGpioPinImplementation<GpioPort_T>::bitNumber_t;
      using bitMask_t = typename GpioPort_T::bitsMask_t;

      inline
      TConstantGpioPin() = default;

      inline GpioPort*
      __attribute__((always_inline))
      getPort(void)
      {
        return nullptr;
      }

      inline static bitNumber_t
      __attribute__((always_inline))
      getBitNumber(void)
      {
        return BIT_NUMBER;
      }

      inline static void
      __attribute__((always_inline))
      configureMode(gpio::Mode value)
      {
        Implementation::configureMode(nullptr, BIT_NUMBER, value);
      }

      inline static gpio::Mode
      __attribute__((always_inline))
      retrieveMode(void)
      {
        return Implementation::configureOutputType(nullptr, BIT_NUMBER);
      }

      inline static void
      __attribute__((always_inline))
      configureOutputType(gpio::OutputType value)
      {
        Implementation::configureOutputType(nullptr, BIT_NUMBER, value);
      }

      inline static gpio::OutputType
      __attribute__((always_inline))
      retrieveOutputType(void)
      {
        return Implementation::retrieveOutputType(nullptr, BIT_NUMBER);
      }

      inline static void
      __attribute__((always_inline))
      configureOutputSpeed(gpio::OutputSpeed value)
      {
        Implementation::configureOutputSpeed(nullptr, BIT_NUMBER, value);
      }

      inline static gpio::OutputSpeed
      __attribute__((always_inline))
      retrieveOutputSpeed(void)
      {
        return Implementation::retrieveOutputSpeed(nullptr, BIT_NUMBER);
      }

      inline static void
      __attribute__((always_inline))
      configureResistors(gpio::Resistors value)
      {
        Implementation::configureResistors(nullptr, BIT_NUMBER, value);
      }

      inline static gpio::Resistors
      __attribute__((always_inline))
      retrieveResistors(void)
      {
        return Implementation::retrieveResistors(nullptr, BIT_NUMBER);
      }

      inline static void
      __attribute__((always_inline))
      configureAlternateFunction(gpio::AlternateFunction value)
      {
        Implementation::configureAlternateFunction(nullptr, BIT_NUMBER, value);
      }

      inline static gpio::AlternateFunction
      __attribute__((always_inline))
      retrieveAlternateFunction(void)
      {
        return Implementation::retrieveAlternateFunction(nullptr, BIT_NUMBER);
      }

      inline static void
      __attribute__((always_inline))
      setHigh(void)
      {
        Implementation::setHigh(nullptr, BIT_MASK);
      }

      inline static void
      __attribute__((always_inline))
      setLow(void)
      {
        Implementation::setLow(nullptr, BIT_MASK);
      }

      inline static void
      __attribute__((always_inline))
      toggle(void)
      {
        Implementation::toggle(nullptr, BIT_MASK);
      }

      inline static bool
      __attribute__((always_inline))
      isOutputLow(void)
      {
        return Implementation::isOutputLow(nullptr, BIT_MASK);
      }

      inline static bool
      __attribute__((always_inline))
      isOutputHigh(void)
      {
        return Implementation::isOutputHigh(nullptr, BIT_MASK);
      }

    private:
      static constexpr bitNumber_t BIT_NUMBER =
          static_cast<bitNumber_t>(bitId_T);
      static constexpr bitMask_t BIT_MASK = (1 << BIT_NUMBER);
    };

// ----------------------------------------------------------------------------
}//namespace stm32f4

// ----------------------------------------------------------------------------

#endif // STM32F4_GPIO_PIN_H_
