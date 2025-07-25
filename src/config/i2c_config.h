#ifndef __I2C_CONF_H__
#define __I2C_CONF_H__

#include "mcu.h"
#include "gpio.h"
#include "gpio_config.h"
#include "rcc.h"

/************************** Settings **************************/
#define I2C_SLAVE_EN                  /* Slave mode enable */
#define I2C_MASTER_INT_EN             /* Master can be interrupted */
/* #define I2C_SUPERVISOR_EN */
/* #define I2C_SUPERVISOR_DELAY		15 */

#define	I2C_MY_SLAVE_ADDR			  0x0A

#define I2C_MASTER_BUFSIZE		  256   /* Размер буфера в режиме мастер */
#define I2C_SLAVE_IN_BUFSIZE    12    /* Размер буфера на прием в режиме слэйв */
#define I2C_SLAVE_OUT_BUFSIZE   12   	/* Размер буфера на передачу в режиме слэйв */
#define I2C_MEM_ADDRSIZE				2			/* Размер буфера адреcа памяти */

#define I2C_PRESC								1
#define I2C_SCLDEL							1
#define I2C_SDADEL							5
#define I2C_SCLH								48
#define I2C_SCLL								65

/************************** End Settings **************************/

#if !defined(I2C_PORT) || !defined(I2C_SDA_PIN_CONF) || !defined(I2C_SCL_PIN_CONF) || \
    !defined(I2C_GPIO_AF) || !defined(I2C_SDA_AF_POS) || !defined(I2C_SCL_AF_POS)
#define I2C_PORT								GPIOA

#define I2C_SDA_PIN_CONF				GPIO_PIN_CONF_10
#define I2C_SCL_PIN_CONF				GPIO_PIN_CONF_9
#define I2C_GPIO_AF							GPIO_AF_4
#define I2C_SDA_AF_POS 					GPIO_POS_AF_PIN_10
#define I2C_SCL_AF_POS 					GPIO_POS_AF_PIN_9
#warning I2C port and pins defined by default
#endif

#endif
