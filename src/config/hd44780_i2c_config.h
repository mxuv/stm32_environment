#ifndef HD44780_I2C_CONFIG_H_
#define HD44780_I2C_CONFIG_H_

/* #define HD44780_I2C_2S */                    /* 2 string display */
#define HD44780_I2C_4S                          /* 4 string dispay */
#define HD44780_I2C_DELAY_BEFORE_INIT_EN        /* enable built-in time delay */

#define HD44780_I2C_ADDR                  0x4E  /* i2c display address*/
#define HD44780_I2C_DELAY_BI_TIME         50    /* ms */
#endif
