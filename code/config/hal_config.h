#ifndef __HAL_CONF_H__
#define __HAL_CONF_H__

/********** MCU config **********/

#define STM32F0
#define STM32F030x6

/********** Clock config **********/

/* #define HSE							  8000000 */
#define F_SYSCLK				  48000000
#define F_HCLK					  48000000
#define F_PCLK					  48000000

/********** External hardware config **********/

#define HD44780_I2C_ADDR  	0x4E
#define AT24CXX_ADDR				0xA0

#endif
