#ifndef __GPIO_CONF_H__
#define __GPIO_CONF_H__

#include "mcu.h"

#define BTN_RED_PORT        GPIOA
#define BTN_GRN_PORT        GPIOB
#define BTN_GRN_PIN         GPIO_PIN_1
#define BTN_RED_PIN         GPIO_PIN_4
#define BTN_GRN_PIN_CONF    GPIO_PIN_CONF_1
#define BTN_RED_PIN_CONF    GPIO_PIN_CONF_4

#define LED_PORT            GPIOF
#define LED_GRN_PIN         GPIO_PIN_0
#define LED_RED_PIN         GPIO_PIN_1
#define LED_GRN_PIN_CONF    GPIO_PIN_CONF_0
#define LED_RED_PIN_CONF    GPIO_PIN_CONF_1

#define I2C_PORT            GPIOA
#define I2C_SDA_PIN_CONF    GPIO_PIN_CONF_10
#define I2C_SCL_PIN_CONF    GPIO_PIN_CONF_9
#define I2C_GPIO_AF         GPIO_AF_4
#define I2C_SDA_AF_POS      GPIO_POS_AF_PIN_10
#define I2C_SCL_AF_POS      GPIO_POS_AF_PIN_9

#define USART_PORT          GPIOA
/* #define USART_RX_PIN      GPIO_PIN_3 */
#define USART_TX_PIN        GPIO_PIN_2
/* #define USART_RX_PIN_CONF GPIO_PIN_CONF_3 */
#define USART_TX_PIN_CONF   GPIO_PIN_CONF_2
#define USART_GPIO_AF       GPIO_AF_1
/* #define USART_RX_AF_POS   GPIO_POS_AF_PIN_3 */
#define USART_TX_AF_POS     GPIO_POS_AF_PIN_2

#endif
