//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

/// \file
/// \brief STM32F4 memory map.

#ifndef STM32F4_MEMORY_MAP_H_
#define STM32F4_MEMORY_MAP_H_

// ----------------------------------------------------------------------------

#include "cortexm/Types.h"

// ----------------------------------------------------------------------------

namespace stm32f4
{
  // --------------------------------------------------------------------------

  /// \ingroup stm32f4
  // \headerfile MemoryMap.h "hal/architecture/arm/cortexm/stm32f/stm32f4/include/MemoryMap.h"
  /// \nosubgrouping
  ///
  /// \brief STM32F4 family memory map.
  class MemoryMap
  {
  public:
    /// FLASH(up to 1 MB) base address in the alias region
    static constexpr cortexm::address_t FLASH_BASE_ADDRESS = 0x08000000ul;

    /// CCM(core coupled memory) data RAM(64 KB) base address in the alias region
    static constexpr cortexm::address_t CCMDATARAM_BASE_ADDRESS = 0x10000000ul;

    /// SRAM1(112 KB) base address in the alias region
    static constexpr cortexm::address_t SRAM1_BASE_ADDRESS = 0x20000000ul;

    /// SRAM2(16 KB) base address in the alias region
    static constexpr cortexm::address_t SRAM2_BASE_ADDRESS = 0x2001C000ul;
    /// SRAM3(64 KB) base address in the alias region
    static constexpr cortexm::address_t SRAM3_BASE_ADDRESS = 0x20020000ul;
    /// Peripheral base address in the alias region
    static constexpr cortexm::address_t PERIPH_BASE_ADDRESS = 0x40000000ul;
    /// Backup SRAM(4 KB) base address in the alias region
    static constexpr cortexm::address_t BKPSRAM_BASE_ADDRESS = 0x40024000ul;
    /// FSMC registers base address
    static constexpr cortexm::address_t FSMC_R_BASE_ADDRESS = 0xA0000000ul;

    /// CCM(core coupled memory) data RAM(64 KB) base address in the bit-band region
    static constexpr cortexm::address_t CCMDATARAM_BB_BASE_ADDRESS =
        0x12000000ul;
    /// SRAM1(112 KB) base address in the bit-band region
    static constexpr cortexm::address_t SRAM1_BB_BASE_ADDRESS = 0x22000000ul;
    /// SRAM2(16 KB) base address in the bit-band region
    static constexpr cortexm::address_t SRAM2_BB_BASE_ADDRESS = 0x2201C000ul;
    /// SRAM3(64 KB) base address in the bit-band region
    static constexpr cortexm::address_t SRAM3_BB_BASE_ADDRESS = 0x22020000ul;
    /// Peripheral base address in the bit-band region
    static constexpr cortexm::address_t PERIPH_BB_BASE_ADDRESS = 0x42000000ul;
    /// Backup SRAM(4 KB) base address in the bit-band region
    static constexpr cortexm::address_t BKPSRAM_BB_BASE_ADDRESS = 0x42024000ul;

    /* Legacy defines */
    static constexpr cortexm::address_t SRAM_BASE_ADDRESS = SRAM1_BASE_ADDRESS;
    static constexpr cortexm::address_t SRAM_BB_BASE_ADDRESS =
        SRAM1_BB_BASE_ADDRESS;

    /// Peripheral memory map
    static constexpr cortexm::registerAddress_t APB1PERIPH_BASE_ADDRESS =
        PERIPH_BASE_ADDRESS;
    static constexpr cortexm::registerAddress_t APB2PERIPH_BASE_ADDRESS =
        (PERIPH_BASE_ADDRESS + 0x00010000ul);
    static constexpr cortexm::registerAddress_t AHB1PERIPH_BASE_ADDRESS =
        (PERIPH_BASE_ADDRESS + 0x00020000ul);
    static constexpr cortexm::registerAddress_t AHB2PERIPH_BASE_ADDRESS =
        (PERIPH_BASE_ADDRESS + 0x10000000ul);

    // APB1 peripherals
    static constexpr cortexm::registerAddress_t TIM2_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x0000ul);
    static constexpr cortexm::registerAddress_t TIM3_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x0400ul);
    static constexpr cortexm::registerAddress_t TIM4_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x0800ul);
    static constexpr cortexm::registerAddress_t TIM5_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x0C00ul);
    static constexpr cortexm::registerAddress_t TIM6_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x1000ul);
    static constexpr cortexm::registerAddress_t TIM7_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x1400ul);
    static constexpr cortexm::registerAddress_t TIM12_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x1800ul);
    static constexpr cortexm::registerAddress_t TIM13_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x1C00ul);
    static constexpr cortexm::registerAddress_t TIM14_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x2000ul);
    static constexpr cortexm::registerAddress_t RTC_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x2800ul);
    static constexpr cortexm::registerAddress_t WWDG_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x2C00ul);
    static constexpr cortexm::registerAddress_t IWDG_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x3000ul);
    static constexpr cortexm::registerAddress_t I2S2ext_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x3400ul);
    static constexpr cortexm::registerAddress_t SPI2_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x3800ul);
    static constexpr cortexm::registerAddress_t SPI3_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x3C00ul);
    static constexpr cortexm::registerAddress_t I2S3ext_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x4000ul);
    static constexpr cortexm::registerAddress_t USART2_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x4400ul);
    static constexpr cortexm::registerAddress_t USART3_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x4800ul);
    static constexpr cortexm::registerAddress_t UART4_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x4C00ul);
    static constexpr cortexm::registerAddress_t UART5_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x5000ul);
    static constexpr cortexm::registerAddress_t I2C1_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x5400ul);
    static constexpr cortexm::registerAddress_t I2C2_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x5800ul);
    static constexpr cortexm::registerAddress_t I2C3_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x5C00ul);
    static constexpr cortexm::registerAddress_t CAN1_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x6400ul);
    static constexpr cortexm::registerAddress_t CAN2_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x6800ul);
    static constexpr cortexm::registerAddress_t PWR_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x7000ul);
    static constexpr cortexm::registerAddress_t DAC_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x7400ul);
    static constexpr cortexm::registerAddress_t UART7_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x7800ul);
    static constexpr cortexm::registerAddress_t UART8_BASE_ADDRESS =
        (APB1PERIPH_BASE_ADDRESS + 0x7C00ul);

    // APB2 peripherals
    static constexpr cortexm::registerAddress_t TIM1_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x0000ul);
    static constexpr cortexm::registerAddress_t TIM8_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x0400ul);
    static constexpr cortexm::registerAddress_t USART1_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x1000ul);
    static constexpr cortexm::registerAddress_t USART6_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x1400ul);
    static constexpr cortexm::registerAddress_t ADC1_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x2000ul);
    static constexpr cortexm::registerAddress_t ADC2_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x2100ul);
    static constexpr cortexm::registerAddress_t ADC3_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x2200ul);
    static constexpr cortexm::registerAddress_t ADC_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x2300ul);
    static constexpr cortexm::registerAddress_t SDIO_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x2C00ul);
    static constexpr cortexm::registerAddress_t SPI1_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x3000ul);
    static constexpr cortexm::registerAddress_t SPI4_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x3400ul);
    static constexpr cortexm::registerAddress_t SYSCFG_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x3800ul);
    static constexpr cortexm::registerAddress_t EXTI_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x3C00ul);
    static constexpr cortexm::registerAddress_t TIM9_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x4000ul);
    static constexpr cortexm::registerAddress_t TIM10_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x4400ul);
    static constexpr cortexm::registerAddress_t TIM11_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x4800ul);
    static constexpr cortexm::registerAddress_t SPI5_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x5000ul);
    static constexpr cortexm::registerAddress_t SPI6_BASE_ADDRESS =
        (APB2PERIPH_BASE_ADDRESS + 0x5400ul);

    // AHB1 peripherals
    static constexpr cortexm::registerAddress_t GPIOA_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x0000ul);
    static constexpr cortexm::registerAddress_t GPIOB_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x0400ul);
    static constexpr cortexm::registerAddress_t GPIOC_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x0800ul);
    static constexpr cortexm::registerAddress_t GPIOD_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x0C00ul);
    static constexpr cortexm::registerAddress_t GPIOE_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x1000ul);
    static constexpr cortexm::registerAddress_t GPIOF_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x1400ul);
    static constexpr cortexm::registerAddress_t GPIOG_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x1800ul);
    static constexpr cortexm::registerAddress_t GPIOH_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x1C00ul);
    static constexpr cortexm::registerAddress_t GPIOI_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x2000ul);
    static constexpr cortexm::registerAddress_t GPIO_OFFSET = GPIOB_BASE_ADDRESS
        - GPIOA_BASE_ADDRESS;

    static constexpr cortexm::registerAddress_t CRC_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x3000ul);
    static constexpr cortexm::registerAddress_t RCC_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x3800ul);
    static constexpr cortexm::registerAddress_t FLASH_R_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x3C00ul);
    static constexpr cortexm::registerAddress_t DMA1_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x6000ul);
    static constexpr cortexm::registerAddress_t DMA1_Stream0_BASE_ADDRESS =
        (DMA1_BASE_ADDRESS + 0x010ul);
    static constexpr cortexm::registerAddress_t DMA1_Stream1_BASE_ADDRESS =
        (DMA1_BASE_ADDRESS + 0x028);
    static constexpr cortexm::registerAddress_t DMA1_Stream2_BASE_ADDRESS =
        (DMA1_BASE_ADDRESS + 0x040ul);
    static constexpr cortexm::registerAddress_t DMA1_Stream3_BASE_ADDRESS =
        (DMA1_BASE_ADDRESS + 0x058);
    static constexpr cortexm::registerAddress_t DMA1_Stream4_BASE_ADDRESS =
        (DMA1_BASE_ADDRESS + 0x070ul);
    static constexpr cortexm::registerAddress_t DMA1_Stream5_BASE_ADDRESS =
        (DMA1_BASE_ADDRESS + 0x088);
    static constexpr cortexm::registerAddress_t DMA1_Stream6_BASE_ADDRESS =
        (DMA1_BASE_ADDRESS + 0x0A0ul);
    static constexpr cortexm::registerAddress_t DMA1_Stream7_BASE_ADDRESS =
        (DMA1_BASE_ADDRESS + 0x0B8);
    static constexpr cortexm::registerAddress_t DMA2_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x6400ul);
    static constexpr cortexm::registerAddress_t DMA2_Stream0_BASE_ADDRESS =
        (DMA2_BASE_ADDRESS + 0x010ul);
    static constexpr cortexm::registerAddress_t DMA2_Stream1_BASE_ADDRESS =
        (DMA2_BASE_ADDRESS + 0x028);
    static constexpr cortexm::registerAddress_t DMA2_Stream2_BASE_ADDRESS =
        (DMA2_BASE_ADDRESS + 0x040ul);
    static constexpr cortexm::registerAddress_t DMA2_Stream3_BASE_ADDRESS =
        (DMA2_BASE_ADDRESS + 0x058UL);
    static constexpr cortexm::registerAddress_t DMA2_Stream4_BASE_ADDRESS =
        (DMA2_BASE_ADDRESS + 0x070ul);
    static constexpr cortexm::registerAddress_t DMA2_Stream5_BASE_ADDRESS =
        (DMA2_BASE_ADDRESS + 0x088);
    static constexpr cortexm::registerAddress_t DMA2_Stream6_BASE_ADDRESS =
        (DMA2_BASE_ADDRESS + 0x0A0ul);
    static constexpr cortexm::registerAddress_t DMA2_Stream7_BASE_ADDRESS =
        (DMA2_BASE_ADDRESS + 0x0B8UL);
    static constexpr cortexm::registerAddress_t ETH_BASE_ADDRESS =
        (AHB1PERIPH_BASE_ADDRESS + 0x8000ul);
    static constexpr cortexm::registerAddress_t ETH_MAC_BASE_ADDRESS =
        (ETH_BASE_ADDRESS);
    static constexpr cortexm::registerAddress_t ETH_MMC_BASE_ADDRESS =
        (ETH_BASE_ADDRESS + 0x0100ul);
    static constexpr cortexm::registerAddress_t ETH_PTP_BASE_ADDRESS =
        (ETH_BASE_ADDRESS + 0x0700ul);
    static constexpr cortexm::registerAddress_t ETH_DMA_BASE_ADDRESS =
        (ETH_BASE_ADDRESS + 0x1000ul);

    // AHB2 peripherals
    static constexpr cortexm::registerAddress_t DCMI_BASE_ADDRESS =
        (AHB2PERIPH_BASE_ADDRESS + 0x50000ul);
    static constexpr cortexm::registerAddress_t CRYP_BASE_ADDRESS =
        (AHB2PERIPH_BASE_ADDRESS + 0x60000ul);
    static constexpr cortexm::registerAddress_t HASH_BASE_ADDRESS =
        (AHB2PERIPH_BASE_ADDRESS + 0x60400ul);
    static constexpr cortexm::registerAddress_t HASH_DIGEST_BASE_ADDRESS =
        (AHB2PERIPH_BASE_ADDRESS + 0x60710ul);
    static constexpr cortexm::registerAddress_t RNG_BASE_ADDRESS =
        (AHB2PERIPH_BASE_ADDRESS + 0x60800ul);

    // FSMC Bankx registers base address
    static constexpr cortexm::registerAddress_t FSMC_Bank1_R_BASE_ADDRESS =
        (FSMC_R_BASE_ADDRESS + 0x0000ul);
    static constexpr cortexm::registerAddress_t FSMC_Bank1E_R_BASE_ADDRESS =
        (FSMC_R_BASE_ADDRESS + 0x0104UL);
    static constexpr cortexm::registerAddress_t FSMC_Bank2_R_BASE_ADDRESS =
        (FSMC_R_BASE_ADDRESS + 0x0060ul);
    static constexpr cortexm::registerAddress_t FSMC_Bank3_R_BASE_ADDRESS =
        (FSMC_R_BASE_ADDRESS + 0x0080ul);
    static constexpr cortexm::registerAddress_t FSMC_Bank4_R_BASE_ADDRESS =
        (FSMC_R_BASE_ADDRESS + 0x00A0ul);

    /// Debug MCU registers base address
    static constexpr cortexm::registerAddress_t DBGMCU_BASE_ADDRESS =
        0xE0042000ul;

  };

} // namespace stm32f4

// --------------------------------------------------------------------------

#endif // STM32F4_MEMORY_MAP_H_
