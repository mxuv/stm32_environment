#ifndef SYSTEM_H
#define SYSTEM_H

#include "mcu.h"

#if defined(STM32F0)
#define FLASH_LATENCY_0						0
#define FLASH_LATENCY_1						FLASH_ACR_LATENCY

#define FLASH_SET_LATENSY_0()			FLASH->ACR &= ~0x07							//SYSCLK < 24 MHz
#define FLASH_SET_LATENSY_1()			FLASH->ACR |= FLASH_LATENCY_1		//24 MHz < SYSCLK < 48 MHz
#endif

#endif
