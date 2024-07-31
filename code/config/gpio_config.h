#ifndef __GPIO_CONF_H__
#define __GPIO_CONF_H__

#include "mcu.h"

#define PWM_PORT		        GPIOA
#define PWM_FAN_PIN			    GPIO_PIN_6
#define PWM_FAN_CONF	      GPIO_PIN_CONF_6
#define PWM_FAN_AF_POS      GPIO_POS_AF_PIN_6
#define PWM_AF              GPIO_AF_5
#define PWM_REG_PIN			    GPIO_PIN_7
#define PWM_REG_CONF	      GPIO_PIN_CONF_7
#define PWM_REG_AF_POS      GPIO_POS_AF_PIN_7



#define ENCODER_PORT        GPIOF
#define ENCODER_PIN_A       GPIO_PIN_1
#define ENCODER_PIN_B       GPIO_PIN_0
#define ENCODER_PIN_CONF_A  GPIO_PIN_1
#define ENCODER_PIN_CONF_B  GPIO_PIN_0

#define OW_PORT             GPIOB
#define OW_PIN_CONF         GPIO_PIN_CONF_1
#define OW_PIN              GPIO_PIN_1

#define BTN_PORT            GPIOA
#define PWR_BTN_PIN         GPIO_PIN_4
#define MODE_BTN_PIN        GPIO_PIN_3
#define ENC_BTN_PIN         GPIO_PIN_5
#define PWR_BTN_PIN_CONF    GPIO_PIN_CONF_4
#define MODE_BTN_PIN_CONF   GPIO_PIN_CONF_5
#define ENC_BTN_PIN_CONF    GPIO_PIN_CONF_9

#define LED_PORT            GPIOA
#define LED_GREEN_PIN       GPIO_PIN_14
#define LED_RED_PIN         GPIO_PIN_13
#define LED_GREEN_PIN_CONF  GPIO_PIN_CONF_14
#define LED_RED_PIN_CONF    GPIO_PIN_CONF_13

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
