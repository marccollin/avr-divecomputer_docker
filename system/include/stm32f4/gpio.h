//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef STM32F4_GPIO_H_
#define STM32F4_GPIO_H_

// ----------------------------------------------------------------------------

#include "cortexm/Types.h"

// ----------------------------------------------------------------------------

namespace stm32f4
{
  namespace gpio
  {
    // ------------------------------------------------------------------------

    using index_t = uint_fast8_t;

    using reg32_t = cortexm::reg32_t;
    using reg16_t = cortexm::reg16_t;

    // mandatory 16 bit!
    using bitsMask_t = uint16_t;

    //using bitValue_t = cortexm::bitband::bitValue_t;

    // ------------------------------------------------------------------------

    using portNumber_t = uint_fast8_t;

    enum class PortId
      : portNumber_t
        {
          A = 0u, //
      B,
      C,
      D,
      E,
      F,
      G,
      H,
      I,
      MAX = I
    };

    // ------------------------------------------------------------------------

    using portBitNumber_t = uint_fast8_t;

    enum class PortBitId
      : portBitNumber_t
        {

          _0 = 0u,
      _1,
      _2,
      _3,
      _4,
      _5,
      _6,
      _7,
      _8,
      _9,
      _10,
      _11,
      _12,
      _13,
      _14,
      _15,
      MAX = _15
    };

    // ------------------------------------------------------------------------

    using mode_t = uint32_t;
    //uint_fast8_t;

    /// \brief Port modes (2 bits/pin).
    enum class Mode
      : mode_t
        {
          Input = 0u, //
      Output = 1u,
      Alternate = 2u,
      Analog = 3u
    };

    /// \brief Mask to isolate the port mode bits.
    constexpr mode_t MODE_MASK = 0x3u;

    // ------------------------------------------------------------------------

    using outputType_t = uint_fast8_t;

    /// \brief Output type (1 bit/pin).
    enum class OutputType
      : outputType_t
        {
          PushPull = 0u, //
      OpenDrain = 1u
    };

    constexpr outputType_t OUTPUT_TYPE_MASK = 0x1u;

    // ------------------------------------------------------------------------

    using outputSpeed_t = uint32_t;

    /// \brief Output Speed (2 bits/pin).
    enum class OutputSpeed
      : outputSpeed_t
        {
          Low_2MHz = 0u, //
      Medium_25MHz = 1u,
      Fast_50MHz = 2u,
      High_100MHz = 3u
    };

    /// \brief Mask to isolate the port output speed bits.
    constexpr outputSpeed_t OUTPUT_SPEED_MASK = 0x3u;

    // ------------------------------------------------------------------------

    using resistors_t = uint32_t;

    /// \brief Pull up/Pull down resistors (2 bits/pin).
    enum class Resistors
      : resistors_t
        {
          None = 0, //
      PullUp = 1,
      PullDown = 2
    };

    /// \brief Mask to isolate the port pull up/pull down bits.
    constexpr resistors_t RESISTORS_MASK = 0x3u;

    // ------------------------------------------------------------------------

    using alternateFunction_t = uint32_t;

    /// \brief Alternate functions (4 bits/pin).
    enum class AlternateFunction
      : alternateFunction_t
        {
          AF0 = 0, //
      AF1,
      AF2,
      AF3,
      AF4,
      AF5,
      AF6,
      AF7,
      AF8,
      AF9,
      AF10,
      AF11,
      AF12,
      AF13,
      AF14,
      AF15
    };

    /// \brief Mask to isolate the alternate function bits.
    static constexpr alternateFunction_t ALTERNATE_FUNCTION_MASK = 0xFu;

  // --------------------------------------------------------------------------
  }// namespace gpio
} // namespace stm32f4

// ----------------------------------------------------------------------------

#endif // STM32F4_GPIO_H_
