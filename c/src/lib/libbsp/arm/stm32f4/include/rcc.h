/*
 * Copyright (c) 2012 Sebastian Huber.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Obere Lagerstr. 30
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.com/license/LICENSE.
 */

#ifndef LIBBSP_ARM_STM32F4_RCC_H
#define LIBBSP_ARM_STM32F4_RCC_H

#include <stdbool.h>

#include <bsp/stm32f4.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define STM32F4_RCC_INDEX(reg, idx) (((reg) << 5) | (idx))

typedef enum {
  STM32F4_RCC_OTGHS = STM32F4_RCC_INDEX(0, 29),
  STM32F4_RCC_ETHMAC = STM32F4_RCC_INDEX(0, 25),
  STM32F4_RCC_DMA2 = STM32F4_RCC_INDEX(0, 22),
  STM32F4_RCC_DMA1 = STM32F4_RCC_INDEX(0, 21),
  STM32F4_RCC_CRC = STM32F4_RCC_INDEX(0, 12),
  STM32F4_RCC_GPIOI = STM32F4_RCC_INDEX(0, 8),
  STM32F4_RCC_GPIOH = STM32F4_RCC_INDEX(0, 7),
  STM32F4_RCC_GPIOG = STM32F4_RCC_INDEX(0, 6),
  STM32F4_RCC_GPIOF = STM32F4_RCC_INDEX(0, 5),
  STM32F4_RCC_GPIOE = STM32F4_RCC_INDEX(0, 4),
  STM32F4_RCC_GPIOD = STM32F4_RCC_INDEX(0, 3),
  STM32F4_RCC_GPIOC = STM32F4_RCC_INDEX(0, 2),
  STM32F4_RCC_GPIOB = STM32F4_RCC_INDEX(0, 1),
  STM32F4_RCC_GPIOA = STM32F4_RCC_INDEX(0, 0),

  STM32F4_RCC_OTGFS = STM32F4_RCC_INDEX(1, 7),
  STM32F4_RCC_RNG = STM32F4_RCC_INDEX(1, 6),
  STM32F4_RCC_HASH = STM32F4_RCC_INDEX(1, 5),
  STM32F4_RCC_CRYP = STM32F4_RCC_INDEX(1, 4),
  STM32F4_RCC_DCMI = STM32F4_RCC_INDEX(1, 0),

  STM32F4_RCC_FSMCR = STM32F4_RCC_INDEX(2, 0),

  STM32F4_RCC_DAC = STM32F4_RCC_INDEX(4, 29),
  STM32F4_RCC_PWR = STM32F4_RCC_INDEX(4, 28),
  STM32F4_RCC_CAN2 = STM32F4_RCC_INDEX(4, 26),
  STM32F4_RCC_CAN1 = STM32F4_RCC_INDEX(4, 25),
  STM32F4_RCC_I2C3 = STM32F4_RCC_INDEX(4, 23),
  STM32F4_RCC_I2C2 = STM32F4_RCC_INDEX(4, 22),
  STM32F4_RCC_I2C1 = STM32F4_RCC_INDEX(4, 21),
  STM32F4_RCC_UART5 = STM32F4_RCC_INDEX(4, 20),
  STM32F4_RCC_UART4 = STM32F4_RCC_INDEX(4, 19),
  STM32F4_RCC_USART3 = STM32F4_RCC_INDEX(4, 18),
  STM32F4_RCC_USART2 = STM32F4_RCC_INDEX(4, 17),
  STM32F4_RCC_SPI3 = STM32F4_RCC_INDEX(4, 15),
  STM32F4_RCC_SPI2 = STM32F4_RCC_INDEX(4, 14),
  STM32F4_RCC_WWDG = STM32F4_RCC_INDEX(4, 11),
  STM32F4_RCC_TIM14 = STM32F4_RCC_INDEX(4, 8),
  STM32F4_RCC_TIM13 = STM32F4_RCC_INDEX(4, 7),
  STM32F4_RCC_TIM12 = STM32F4_RCC_INDEX(4, 6),
  STM32F4_RCC_TIM7 = STM32F4_RCC_INDEX(4, 5),
  STM32F4_RCC_TIM6 = STM32F4_RCC_INDEX(4, 4),
  STM32F4_RCC_TIM5 = STM32F4_RCC_INDEX(4, 3),
  STM32F4_RCC_TIM4 = STM32F4_RCC_INDEX(4, 2),
  STM32F4_RCC_TIM3 = STM32F4_RCC_INDEX(4, 1),
  STM32F4_RCC_TIM2 = STM32F4_RCC_INDEX(4, 0),

  STM32F4_RCC_TIM11 = STM32F4_RCC_INDEX(5, 18),
  STM32F4_RCC_TIM10 = STM32F4_RCC_INDEX(5, 17),
  STM32F4_RCC_TIM9 = STM32F4_RCC_INDEX(5, 16),
  STM32F4_RCC_SYSCFG = STM32F4_RCC_INDEX(5, 14),
  STM32F4_RCC_SPI1 = STM32F4_RCC_INDEX(5, 12),
  STM32F4_RCC_SDIO = STM32F4_RCC_INDEX(5, 11),
  STM32F4_RCC_ADC3 = STM32F4_RCC_INDEX(5, 10),
  STM32F4_RCC_ADC2 = STM32F4_RCC_INDEX(5, 9),
  STM32F4_RCC_ADC1 = STM32F4_RCC_INDEX(5, 8),
  STM32F4_RCC_USART6 = STM32F4_RCC_INDEX(5, 5),
  STM32F4_RCC_USART1 = STM32F4_RCC_INDEX(5, 4),
  STM32F4_RCC_TIM8 = STM32F4_RCC_INDEX(5, 1),
  STM32F4_RCC_TIM1 = STM32F4_RCC_INDEX(5, 0),
} stm32f4_rcc_index;

void stm32f4_rcc_reset(stm32f4_rcc_index index, bool set);

void stm32f4_rcc_set_clock(stm32f4_rcc_index index, bool set);

void stm32f4_rcc_set_gpio_clock(int pin, bool set);

void stm32f4_rcc_set_low_power_clock(stm32f4_rcc_index index, bool set);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBBSP_ARM_STM32F4_RCC_H */
