#ifndef __RCC_H__
#define __RCC_H__

#include "mcu.h"

#if defined(STM32F0)
#define HSE_ON()										RCC->CR |= RCC_CR_HSEON
#define HSE_RDY											(RCC->CR & RCC_CR_HSERDY)
#define HSI_14_ON()                 RCC->CR2 |= RCC_CR2_HSI14ON
#define HSI_14_RDY                  (RCC->CR2 & RCC_CR2_HSI14RDY)

#define PLL_SRC_HSI_DIV2 						RCC_CFGR_PLLSRC_HSI_DIV2
#define PLL_SRC_HSE_PREDIV 					RCC_CFGR_PLLSRC_HSE_PREDIV

#define PLL_HSE_PREDIV_1						RCC_CFGR2_PREDIV_DIV1
#define PLL_HSE_PREDIV_2						RCC_CFGR2_PREDIV_DIV2
#define PLL_HSE_PREDIV_3						RCC_CFGR2_PREDIV_DIV3
#define PLL_HSE_PREDIV_4						RCC_CFGR2_PREDIV_DIV4
#define PLL_HSE_PREDIV_5						RCC_CFGR2_PREDIV_DIV5
#define PLL_HSE_PREDIV_6						RCC_CFGR2_PREDIV_DIV6
#define PLL_HSE_PREDIV_7						RCC_CFGR2_PREDIV_DIV7
#define PLL_HSE_PREDIV_8						RCC_CFGR2_PREDIV_DIV8
#define PLL_HSE_PREDIV_9						RCC_CFGR2_PREDIV_DIV9
#define PLL_HSE_PREDIV_10						RCC_CFGR2_PREDIV_DIV10
#define PLL_HSE_PREDIV_11						RCC_CFGR2_PREDIV_DIV11
#define PLL_HSE_PREDIV_12						RCC_CFGR2_PREDIV_DIV12
#define PLL_HSE_PREDIV_13						RCC_CFGR2_PREDIV_DIV13
#define PLL_HSE_PREDIV_14						RCC_CFGR2_PREDIV_DIV14
#define PLL_HSE_PREDIV_15						RCC_CFGR2_PREDIV_DIV15
#define PLL_HSE_PREDIV_16						RCC_CFGR2_PREDIV_DIV16

#define PLL_MUL_2										RCC_CFGR_PLLMUL2
#define PLL_MUL_3										RCC_CFGR_PLLMUL3
#define PLL_MUL_4										RCC_CFGR_PLLMUL4
#define PLL_MUL_5										RCC_CFGR_PLLMUL5
#define PLL_MUL_6										RCC_CFGR_PLLMUL6
#define PLL_MUL_7										RCC_CFGR_PLLMUL7
#define PLL_MUL_8										RCC_CFGR_PLLMUL8
#define PLL_MUL_9										RCC_CFGR_PLLMUL9
#define PLL_MUL_10									RCC_CFGR_PLLMUL10
#define PLL_MUL_11									RCC_CFGR_PLLMUL11
#define PLL_MUL_12									RCC_CFGR_PLLMUL12
#define PLL_MUL_13									RCC_CFGR_PLLMUL13
#define PLL_MUL_14									RCC_CFGR_PLLMUL14
#define PLL_MUL_15									RCC_CFGR_PLLMUL15
#define PLL_MUL_16									RCC_CFGR_PLLMUL16

#define PLL_SET_SRC(src)						RCC->CFGR |= (src)
#define PLL_SET_PREDIV(div)					RCC->CFGR2 |= (div)
#define PLL_SET_MUL(mul)						RCC->CFGR |= (mul)

#define PLL_ON()										RCC->CR |= RCC_CR_PLLON
#define PLL_RDY											(RCC->CR & RCC_CR_PLLRDY)

#define HCLK_DIV_1									RCC_CFGR_HPRE_DIV1
#define HCLK_DIV_2									RCC_CFGR_HPRE_DIV2
#define HCLK_DIV_4									RCC_CFGR_HPRE_DIV4
#define HCLK_DIV_8									RCC_CFGR_HPRE_DIV8
#define HCLK_DIV_16									RCC_CFGR_HPRE_DIV16
#define HCLK_DIV_64									RCC_CFGR_HPRE_DIV64
#define HCLK_DIV_128								RCC_CFGR_HPRE_DIV128
#define HCLK_DIV_256								RCC_CFGR_HPRE_DIV256
#define HCLK_DIV_512								RCC_CFGR_HPRE_DIV512

#define AHB_SET_PRESCALER(div)			RCC->CFGR |= (div)

#define PCLK_DIV_1									RCC_CFGR_PPRE_DIV1
#define PCLK_DIV_2									RCC_CFGR_PPRE_DIV2
#define PCLK_DIV_4									RCC_CFGR_PPRE_DIV4
#define PCLK_DIV_8									RCC_CFGR_PPRE_DIV8
#define PCLK_DIV_16									RCC_CFGR_PPRE_DIV16

#define APB_SET_PRESCALER(div)			RCC->CFGR |= (div)

#define SYSCLK_SRC_HSI							RCC_CFGR_SW_HSI
#define SYSCLK_SRC_HSE							RCC_CFGR_SW_HSE
#define SYSCLK_SRC_PLL							RCC_CFGR_SW_PLL

#define SYSCLK_SWS_HSI							RCC_CFGR_SWS_HSI
#define SYSCLK_SWS_HSE							RCC_CFGR_SWS_HSE
#define SYSCLK_SWS_PLL							RCC_CFGR_SWS_PLL

#define SYSCLK_SET_SRC(src)					RCC->CFGR |= (src)
#define SYSCLK_SWS									(RCC->CFGR & SYSCLK_SWS_PLL)

#define RCC_USART1_CLK_SRC_PCLK			RCC_CFGR3_USART1SW_PCLK
#define RCC_USART1_CLK_SRC_HSI			RCC_CFGR3_USART1SW_HSI
#define RCC_USART1_CLK_SRC_LSE			RCC_CFGR3_USART1SW_LSE
#define RCC_USART1_CLK_SRC_SYSCLK		RCC_CFGR3_USART1SW_SYSCLK

#define RCC_USART1_CLK_SRC(src)			RCC->CFGR3 |= (src)

#define RCC_I2C1_CLK_SRC_SYSCLK()		RCC->CFGR3 |= RCC_CFGR3_I2C1SW_SYSCLK
#define RCC_I2C1_CLK_SRC_HSI()			RCC->CFGR3 &= ~RCC_CFGR3_I2C1SW_SYSCLK

#define RCC_CLKEN_GPIOF()						RCC->AHBENR	|= RCC_AHBENR_GPIOFEN
#define RCC_CLKEN_GPIOC()						RCC->AHBENR	|= RCC_AHBENR_GPIOCEN
#define RCC_CLKEN_GPIOB()						RCC->AHBENR	|= RCC_AHBENR_GPIOBEN
#define RCC_CLKEN_GPIOA()						RCC->AHBENR |= RCC_AHBENR_GPIOAEN

#define RCC_CLKEN_USART1()					RCC->APB2ENR |= RCC_APB2ENR_USART1EN
#define RCC_CLKEN_I2C1()						RCC->APB1ENR |= RCC_APB1ENR_I2C1EN
#define RCC_CLKEN_SPI1()						RCC->APB2ENR |= RCC_APB2ENR_SPI1EN

#define RCC_CLKEN_TIM14()           RCC->APB1ENR |= RCC_APB1ENR_TIM14EN
#define RCC_CLKEN_TIM16()           RCC->APB2ENR |= RCC_APB2ENR_TIM16EN
#define RCC_CLKEN_TIM17()						RCC->APB2ENR |= RCC_APB2ENR_TIM17EN

#define RCC_CLKEN_ADC()             RCC->APB2ENR |= RCC_APB2ENR_ADCEN
#define RCC_CLKEN_DMA()             RCC->AHBENR |= RCC_AHBENR_DMAEN
#endif

#endif
