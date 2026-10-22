//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef STM32F4_RCC_H_
#define STM32F4_RCC_H_

// ----------------------------------------------------------------------------

#include "cortexm/BitBand.h"
#include "stm32f4/MemoryMap.h"
#include "stm32f4/GpioPort.h"

// ----------------------------------------------------------------------------

namespace stm32f4
{
  namespace rcc
  {
    using bitNumber_t = cortexm::bitNumber_t;

    /// \name Memory definitions
    /// @{

    /// \brief Base address of the device.
    constexpr cortexm::registerAddress_t MEMORY_BASE_ADDRESS =
        MemoryMap::RCC_BASE_ADDRESS;

    /// \brief 0x0030 - AHB1 peripheral clock register.
    constexpr cortexm::registerAddress_t AHB1ENR_ADDRESS = MEMORY_BASE_ADDRESS
        + 0x0030u;

    /// @} end of name Memory definitions

    /// \addtogroup stm32f4_rcc
    /// @{

    // ----------------------------------------------------------------------

    /// \brief AHB1 peripheral number type definition.
    typedef rcc::bitNumber_t Ahb1Peripheral_t;

    /// \brief AHB1 peripheral numbers.
    ///
    /// \details
    /// Use this when calling AHB1 related calls.
    /// \par Example
    /// \code{.cpp}
    /// Rcc::enableAhb1PeripheralClock(rcc::Ahb1Peripheral::GpioA);
    /// \endcode
    enum class Ahb1Peripheral
      : Ahb1Peripheral_t
        {
          GpioA = 0, //
      GpioB = 1,
      GpioC = 2,
      GpioD = 3,
      GpioE = 4,
      GpioF = 5,
      GpioH = 6,
      GpioI = 7,
      Crc = 12,
      BkpSram = 18,
      CcmDataRam = 20,
      Dma1 = 21,
      Dma2 = 22,
      EthMac = 25,
      EthMacTx = 26,
      EthMacRx = 27,
      EthMacPtp,
      OtgHs = 29,
      OtgHsUlpi = 30
    };

  // ----------------------------------------------------------------------

  /// \brief Reload register type.
  //typedef uint16_t reloadValue_t;

  // ----------------------------------------------------------------------

  /// @} end of addtogroup stm32f4_rcc

  }// namespace rcc
} //namespace stm32f4

namespace stm32f4
{
  // \headerfile Rcc.h "hal/architecture/arm/cortexm/stm32f/stm32f4/peripheral/include/Rcc.h"
  /// \ingroup stm32f4_rcc
  /// \nosubgrouping
  ///
  /// \brief Reset and Clock Control class template (static inline version).
  ///
  /// \details
  /// This class  provides control of the STM32F4 RCC.
  ///
  /// For performance reasons, all member functions are inline and very
  /// simple, performing just the required memory access(es).
  ///
  /// Using it is typical for static classes, although a default
  /// constructor is provided, so you can instantiate it as well.
  ///
  /// \par Example
  /// \code{.cpp}
  /// Rcc::enableAhb1PeripheralClock(rcc::Ahb1Peripheral::GpioA);
  /// \endcode
  ///

  class Rcc
  {
  public:
    Rcc() = default;

    /// \name Configuration functions
    /// @{

    /// \brief Enable the clock for one AHB1 connected peripheral.
    ///
    /// \param [in] peripheral The peripheral number.
    /// \par Returns
    ///    Nothing.
    inline static void
    __attribute__((always_inline))
    enableAhb1PeripheralClock(rcc::Ahb1Peripheral peripheral)
    {
      rcc::bitNumber_t bitNumber = static_cast<rcc::bitNumber_t>(peripheral);

      cortexm::BitBand::setPeripheralBit(rcc::AHB1ENR_ADDRESS, bitNumber);
    }

    /// \brief Disable the clock for one AHB1 connected peripheral.
    ///
    /// \param [in] peripheral The peripheral number.
    /// \par Returns
    ///    Nothing.
    inline static void
    __attribute__((always_inline))
    disableAhb1PeripheralClock(const rcc::Ahb1Peripheral peripheral)
    {
      rcc::bitNumber_t bitNumber = static_cast<rcc::bitNumber_t>(peripheral);

      cortexm::BitBand::clearPeripheralBit(rcc::AHB1ENR_ADDRESS, bitNumber);
    }

    /// \brief Tell if the clock of the AHB1 connected peripheral is enabled.
    ///
    /// \param [in] peripheral The peripheral number.
    /// \par Returns
    ///    Nothing.
    inline static bool
    __attribute__((always_inline))
    isAhb1PeripheralClockEnabled(rcc::Ahb1Peripheral peripheral)
    {
      rcc::bitNumber_t bitNumber = static_cast<rcc::bitNumber_t>(peripheral);

      return (cortexm::BitBand::readPeripheralBitValue(rcc::AHB1ENR_ADDRESS,
          bitNumber) != 0);
    }

    /// \brief Enable the clock for one GPIO port.
    ///
    /// \param [in] port The GPIO port.
    /// \par Returns
    ///    Nothing.
    inline static void
    __attribute__((always_inline))
    enableGpioPeripheralClock(const gpio::PortId port)
    {
      // GPIO port numbers are successive
      rcc::bitNumber_t bitNumber =
          static_cast<rcc::bitNumber_t>(rcc::Ahb1Peripheral::GpioA);
      bitNumber += static_cast<rcc::bitNumber_t>(port);
      enableAhb1PeripheralClock(static_cast<rcc::Ahb1Peripheral>(bitNumber));
    }

    /// \brief Disable the clock for one GPIO port.
    ///
    /// \param [in] port The GPIO port.
    /// \par Returns
    ///    Nothing.
    inline static void
    __attribute__((always_inline))
    disableGpioPeripheralClock(const gpio::PortId port)
    {
      // GPIO port numbers are successive
      rcc::bitNumber_t bitNumber =
          static_cast<rcc::bitNumber_t>(rcc::Ahb1Peripheral::GpioA);
      bitNumber += static_cast<rcc::bitNumber_t>(port);
      disableAhb1PeripheralClock(static_cast<rcc::Ahb1Peripheral>(bitNumber));
    }

    /// @} end of name Configuration functions

  };

// ----------------------------------------------------------------------------
}// namespace stm32f4

// ----------------------------------------------------------------------------

#endif // STM32F4_RCC_H_
