#include <stdint.h>

#include "gpio.h"
#include "mcu.h"

void gpio_pin_config(GPIO_TypeDef *GPIOx, uint32_t mode, uint32_t type,
    uint32_t pull, uint32_t speed, uint32_t pin)
{
  GPIOx->OTYPER = (GPIOx->OTYPER & ~(0x01 << pin / 2)) | GPIO_OUT(type, pin);
  GPIOx->PUPDR = (GPIOx->PUPDR & ~(0x03 << pin)) | GPIO_PULL(pull, pin);
  GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~(0x03 << pin)) | GPIO_SPEED(speed, pin);
  GPIOx->MODER = (GPIOx->MODER & ~(0x03 << pin)) | GPIO_MODE(mode, pin);
}
