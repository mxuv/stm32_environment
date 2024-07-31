#ifndef __USART_CONF_H__
#define __USART_CONF_H__

#include "mcu.h"
#include "gpio.h"
#include "gpio_config.h"
#include "rcc.h"

#define USART_TX_EN
#define USART_RX_EN

#define USART           USART1
#define USART_BAUDRATE  115200
#define USART_BAUDDIV   F_SYSCLK/USART_BAUDRATE

#define USART_INBUF_SIZE    1
#define USART_OUTBUF_SIZE   10

#endif
