#ifndef __HD44780_I2C_H__
#define __HD44780_I2C_H__

#include "hal_config.h"
#include "hardware_module_config.h"

#ifdef HD44780_I2C_EN

#ifndef HD44780_I2C_ADDR
#define HD44780_I2C_ADDR  				0x4E
#endif

#define HD44780_I2C_RS   				0b00000001
#define HD44780_I2C_E    				0b00000100
#define HD44780_I2C_Light_PIN		0b00001000

#define HD44780_I2C_PMSK 				0b00001111

#define HD44780_I2C_MBlink	    0b00000001
#define HD44780_I2C_MCur  	    0b00000010
#define HD44780_I2C_MDisplayOn	0b00000100

#define HD44780_I2C_Processed		0b10000000
#define HD44780_I2C_Free     		0b01111111
#define HD44780_I2C_Init1    		0b01000000
#define HD44780_I2C_Init2    		0b00100000
#define HD44780_I2C_LightEn			0b00000001
#define HD44780_I2C_LightProc		0b00000010
#define HD44780_I2C_LightMsk		0b00000011

#define HD44780_I2C_Light_ON		1
#define HD44780_I2C_Light_OFF		0

void hd44780_i2c_Init(void);
void hd44780_i2c_InitNxt(void);
void hd44780_i2c_Ok(void);
void hd44780_i2c_Tx(uint8_t data, uint8_t EnDat);
void hd44780_i2c_Refresh(void);
void hd44780_i2c_SetCursor(uint8_t X, uint8_t Y);
void hd44780_i2c_LightOff(void);
void hd44780_i2c_LightOn(void);
void hd44780_i2c_LightSw(void);
void hd44780_i2c_SendStringC(uint8_t f_ch, uint8_t n_ch, const char *str);
void hd44780_i2c_SendString(const char *str);
void hd44780_i2c_SendChar(uint8_t ch);
void hd44780_i2c_Clear(void);

#endif

#endif
