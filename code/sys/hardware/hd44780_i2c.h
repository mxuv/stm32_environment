#ifndef __HD44780_I2C_H__
#define __HD44780_I2C_H__

#include "hw_config.h"
#include "hal_config.h"

#ifdef HD44780_I2C_EN

#ifndef HD44780_I2C_ADDR
#error Undefined HD44780_I2C_ADDR
#endif

#define HD44780_I2C_RS              0b00000001
#define HD44780_I2C_E               0b00000100
#define HD44780_I2C_LIGHT_PIN       0b00001000

#define HD44780_I2C_PMSK            0b00001111

#define HD44780_I2C_MBLINK          0b00000001
#define HD44780_I2C_MCUR            0b00000010
#define HD44780_I2C_MDISPLAY_ON     0b00000100

#define HD44780_I2C_PROCESSED       0b10000000
#define HD44780_I2C_FREE            0b01111111
#define HD44780_I2C_INIT1           0b01000000
#define HD44780_I2C_INIT2           0b00100000
#define HD44780_I2C_LIGHT_EN        0b00000001
#define HD44780_I2C_LIGHT_PROC      0b00000010
#define HD44780_I2C_LIGHT_MSK       0b00000011

#define HD44780_I2C_LIGHT_ON        1
#define HD44780_I2C_LIGHT_OFF       0

void hd44780_i2c_init(void);
void hd44780_i2c_refresh(void);
void hd44780_i2c_setcursor(uint8_t x, uint8_t y);
void hd44780_i2c_lightoff(void);
void hd44780_i2c_lighton(void);
void hd44780_i2c_sendstringc(uint8_t f_ch, uint8_t n_ch, const char *str);
void hd44780_i2c_sendstring(const char *str);
void hd44780_i2c_sendchar(uint8_t ch);
void hd44780_i2c_clear(void);

#endif

#endif
