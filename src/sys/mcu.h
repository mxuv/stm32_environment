#ifndef __MCU_H__
#define __MCU_H__

#include "hal_config.h"

#if defined(STM32F0)
#include "stm32f0xx.h"
#endif

#if defined(STM32F0)

#define VREF_INTCAL_ADDR  0x1FFFF7BA

#define ENABLE_INTERRUPTS() 	__enable_irq();
#define DISABLE_INTERRUPTS() 	__disable_irq();
#endif

#endif
