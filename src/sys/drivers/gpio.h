#ifndef __GPIO_H__
#define __GPIO_H__

#include "mcu.h"

#if defined(STM32F0)
#define GPIO_PIN_CONF_0         	(0U)
#define GPIO_PIN_CONF_1         	(2U)
#define GPIO_PIN_CONF_2        		(4U)
#define GPIO_PIN_CONF_3        		(6U)
#define GPIO_PIN_CONF_4        		(8U)
#define GPIO_PIN_CONF_5        		(10U)
#define GPIO_PIN_CONF_6         	(12U)
#define GPIO_PIN_CONF_7          	(14U)
#define GPIO_PIN_CONF_8          	(16U)
#define GPIO_PIN_CONF_9          	(18U)
#define GPIO_PIN_CONF_10         	(20U)
#define GPIO_PIN_CONF_11         	(22U)
#define GPIO_PIN_CONF_12         	(24U)
#define GPIO_PIN_CONF_13        	(26U)
#define GPIO_PIN_CONF_14         	(28U)
#define GPIO_PIN_CONF_15         	(30U)

#define GPIO_PIN_0          			GPIO_BSRR_BS_0
#define GPIO_PIN_1    			      GPIO_BSRR_BS_1
#define GPIO_PIN_2    			      GPIO_BSRR_BS_2
#define GPIO_PIN_3    			      GPIO_BSRR_BS_3
#define GPIO_PIN_4   			      	GPIO_BSRR_BS_4
#define GPIO_PIN_5     			     	GPIO_BSRR_BS_5
#define GPIO_PIN_6     			     	GPIO_BSRR_BS_6
#define GPIO_PIN_7      			    GPIO_BSRR_BS_7
#define GPIO_PIN_8      			    GPIO_BSRR_BS_8
#define GPIO_PIN_9      			    GPIO_BSRR_BS_9
#define GPIO_PIN_10      			   	GPIO_BSRR_BS_10
#define GPIO_PIN_11       			  GPIO_BSRR_BS_11
#define GPIO_PIN_12      			   	GPIO_BSRR_BS_12
#define GPIO_PIN_13       			  GPIO_BSRR_BS_13
#define GPIO_PIN_14       			  GPIO_BSRR_BS_14
#define GPIO_PIN_15        		 		GPIO_BSRR_BS_15

#define GPIO_POS_AF_PIN_0					(0U)
#define GPIO_POS_AF_PIN_1					(4U)
#define GPIO_POS_AF_PIN_2					(8U)
#define GPIO_POS_AF_PIN_3					(12U)
#define GPIO_POS_AF_PIN_4					(16U)
#define GPIO_POS_AF_PIN_5					(20U)
#define GPIO_POS_AF_PIN_6					(24U)
#define GPIO_POS_AF_PIN_7					(28U)

#define GPIO_POS_AF_PIN_8 				GPIO_POS_AF_PIN_0
#define GPIO_POS_AF_PIN_9 				GPIO_POS_AF_PIN_1
#define GPIO_POS_AF_PIN_10 				GPIO_POS_AF_PIN_2
#define GPIO_POS_AF_PIN_11				GPIO_POS_AF_PIN_3
#define GPIO_POS_AF_PIN_12 				GPIO_POS_AF_PIN_4
#define GPIO_POS_AF_PIN_13 				GPIO_POS_AF_PIN_5
#define GPIO_POS_AF_PIN_14 				GPIO_POS_AF_PIN_6
#define GPIO_POS_AF_PIN_15 				GPIO_POS_AF_PIN_7

#define GPIO_MODE_IN							(0x00000000U)							//In (0)
#define GPIO_MODE_OUT							GPIO_MODER_MODER0_0				//Out (1)
#define GPIO_MODE_AF							GPIO_MODER_MODER0_1				//Alternate function (2)
#define GPIO_MODE_AN							GPIO_MODER_MODER0					//Analog input (3)

#define GPIO_MODE0_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_0
#define GPIO_MODE0_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_0
#define GPIO_MODE0_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_0
#define GPIO_MODE0_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_0
#define GPIO_MODE1_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_1
#define GPIO_MODE1_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_1
#define GPIO_MODE1_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_1
#define GPIO_MODE1_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_1
#define GPIO_MODE2_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_2
#define GPIO_MODE2_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_2
#define GPIO_MODE2_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_2
#define GPIO_MODE2_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_2
#define GPIO_MODE3_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_3
#define GPIO_MODE3_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_3
#define GPIO_MODE3_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_3
#define GPIO_MODE3_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_3
#define GPIO_MODE4_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_4
#define GPIO_MODE4_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_4
#define GPIO_MODE4_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_4
#define GPIO_MODE4_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_4
#define GPIO_MODE5_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_5
#define GPIO_MODE5_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_5
#define GPIO_MODE5_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_5
#define GPIO_MODE5_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_5
#define GPIO_MODE6_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_6
#define GPIO_MODE6_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_6
#define GPIO_MODE6_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_6
#define GPIO_MODE6_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_6
#define GPIO_MODE7_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_7
#define GPIO_MODE7_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_7
#define GPIO_MODE7_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_7
#define GPIO_MODE7_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_7
#define GPIO_MODE8_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_8
#define GPIO_MODE8_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_8
#define GPIO_MODE8_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_8
#define GPIO_MODE8_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_8
#define GPIO_MODE9_IN							GPIO_MODE_IN 				<< GPIO_PIN_CONF_9
#define GPIO_MODE9_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_9
#define GPIO_MODE9_AF							GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_9
#define GPIO_MODE9_AN							GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_9
#define GPIO_MODE10_IN						GPIO_MODE_IN 				<< GPIO_PIN_CONF_10
#define GPIO_MODE10_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_10
#define GPIO_MODE10_AF						GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_10
#define GPIO_MODE10_AN						GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_10
#define GPIO_MODE11_IN						GPIO_MODE_IN 				<< GPIO_PIN_CONF_11
#define GPIO_MODE11_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_11
#define GPIO_MODE11_AF						GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_11
#define GPIO_MODE11_AN						GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_11
#define GPIO_MODE12_IN						GPIO_MODE_IN 				<< GPIO_PIN_CONF_12
#define GPIO_MODE12_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_12
#define GPIO_MODE12_AF						GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_12
#define GPIO_MODE12_AN						GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_12
#define GPIO_MODE13_IN						GPIO_MODE_IN 				<< GPIO_PIN_CONF_13
#define GPIO_MODE13_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_13
#define GPIO_MODE13_AF						GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_13
#define GPIO_MODE13_AN						GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_13
#define GPIO_MODE14_IN						GPIO_MODE_IN 				<< GPIO_PIN_CONF_14
#define GPIO_MODE14_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_14
#define GPIO_MODE14_AF						GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_14
#define GPIO_MODE14_AN						GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_14
#define GPIO_MODE15_IN						GPIO_MODE_IN 				<< GPIO_PIN_CONF_15
#define GPIO_MODE15_OUT						GPIO_MODER_MODER0_0 << GPIO_PIN_CONF_15
#define GPIO_MODE15_AF						GPIO_MODER_MODER0_1 << GPIO_PIN_CONF_15
#define GPIO_MODE15_AN						GPIO_MODER_MODER0 	<< GPIO_PIN_CONF_15

#define GPIO_MODE(MODE, PIN)			(MODE << PIN)

#define GPIO_OUT_PP      					(0x00000000U) 						//Output push-pull (0)
#define GPIO_OUT_OD    						GPIO_OTYPER_OT_0 					//Output open-drain (1)

#define GPIO_OUT(TYPE, PIN)				(TYPE << PIN)

#define GPIO_SPEED_LOW    				(0x00000000U) 						//(0)
#define GPIO_SPEED_MEDIUM   			GPIO_OSPEEDR_OSPEEDR0_0		//(1)
#define GPIO_SPEED_HIGH     			GPIO_OSPEEDR_OSPEEDR0			//(2)

#define GPIO_SPEED(SPEED, PIN)		(SPEED << PIN)

#define GPIO_PULL_NO        			(0x00000000U)							//(0)
#define GPIO_PULL_UP       				GPIO_PUPDR_PUPDR0_0				//(1)
#define GPIO_PULL_DOWN      			GPIO_PUPDR_PUPDR0_1				//(2)

#define GPIO_PULL(PULL, PIN)			(PULL << PIN)

#define GPIO_AF_0           (0x0000000U)
#define GPIO_AF_1           (0x0000001U)
#define GPIO_AF_2           (0x0000002U)
#define GPIO_AF_3           (0x0000003U)
#define GPIO_AF_4           (0x0000004U)
#define GPIO_AF_5           (0x0000005U)
#define GPIO_AF_6           (0x0000006U)
#define GPIO_AF_7           (0x0000007U)

#define GPIO_SET_MODE				__GPIO_SetMode
#define GPIO_SET_OUT_TYPE		__GPIO_SetOType
#define GPIO_SET_PULL				__GPIO_SetPull
#define GPIO_SET_SPEED			__GPIO_SetSpeed

#define GPIO_PIN_SET				__GPIO_PIN_SET
#define GPIO_PIN_RESET			__GPIO_PIN_RESET

#define GPIO_GET_PIN       __GPIO_GetPin

__STATIC_INLINE void __GPIO_PIN_SET(GPIO_TypeDef *GPIOx, uint32_t pin)
{
  GPIOx->BSRR |= pin;
}

__STATIC_INLINE void __GPIO_PIN_RESET(GPIO_TypeDef *GPIOx, uint32_t pin)
{
  GPIOx->BRR |= pin;
}

/* Установить режим работы: вход/выход/альт функция/аналоговый вход */

__STATIC_INLINE void __GPIO_SetMode(GPIO_TypeDef *GPIOx, uint32_t mode,
    uint32_t pin)
{
  GPIOx->MODER = (GPIOx->MODER & ~(0x03 << pin)) | GPIO_MODE(mode, pin);
}

/* Установить режим выхода: пушпул/открытый сток */

__STATIC_INLINE void __GPIO_SetOType(GPIO_TypeDef *GPIOx, uint32_t Type,
    uint32_t pin)
{
  GPIOx->OTYPER = (GPIOx->OTYPER & ~(0x01 << pin / 2)) | GPIO_OUT(Type, pin);
}

/* Установить режим подтяжки: к плюсу/к минусу/без подтяжки */

__STATIC_INLINE void __GPIO_SetPull(GPIO_TypeDef *GPIOx, uint32_t pull,
    uint32_t pin)
{
  GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << pin)) | GPIO_PULL(pull, pin);
}

__STATIC_INLINE void __GPIO_SetSpeed(GPIO_TypeDef *GPIOx, uint32_t speed,
    uint32_t pin)
{
  GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~(0x03 << pin)) | GPIO_SPEED(speed, pin);
}

void gpio_pin_config(GPIO_TypeDef *GPIOx, uint32_t mode, uint32_t type,
    uint32_t pull, uint32_t speed, uint32_t pin);

__STATIC_INLINE uint32_t __GPIO_GetPin(GPIO_TypeDef *GPIOx, uint32_t pin)
{
  return (uint32_t) (GPIOx->IDR & pin);
}

#endif

#endif
