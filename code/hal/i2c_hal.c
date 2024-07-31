#include "i2c_config.h"
#include "i2c_hal.h"

void i2c_init(void)
{
  RCC_CLKEN_I2C1();

  I2C_PORT->AFR[1] |= I2C_GPIO_AF << I2C_SCL_AF_POS
      | I2C_GPIO_AF << I2C_SDA_AF_POS;

  gpio_pin_config(I2C_PORT, GPIO_MODE_AF, GPIO_OUT_OD, GPIO_PULL_UP,
  GPIO_SPEED_HIGH, I2C_SCL_PIN_CONF);

  gpio_pin_config(I2C_PORT, GPIO_MODE_AF, GPIO_OUT_OD, GPIO_PULL_UP,
  GPIO_SPEED_HIGH, I2C_SDA_PIN_CONF);

  I2C1->TIMINGR = I2C_TIMINGR_VAL;

#ifdef I2C_SLAVE_EN
  I2C1->OAR1 |= (uint32_t) (I2C_MY_SLAVE_ADDR << 1) | I2C_OAR1_OA1EN;
  I2C1->CR1 |= I2C_CR1_ADDRIE;
#endif

  I2C1->CR1 |= I2C_CR1_TCIE | I2C_CR1_TXIE | I2C_CR1_ERRIE | I2C_CR1_RXIE
      | I2C_CR1_STOPIE | I2C_CR1_NACKIE;
  I2C1->CR1 |= I2C_CR1_PE;

  NVIC_EnableIRQ(I2C1_IRQn);
}

void i2c_module_reinit(void)
{
  I2C1->CR1 = 0;

#ifdef I2C_SLAVE_EN
  I2C1->CR1 |= I2C_CR1_ADDRIE;
#endif

  I2C1->CR1 |= I2C_CR1_TCIE | I2C_CR1_TXIE | I2C_CR1_ERRIE | I2C_CR1_RXIE
      | I2C_CR1_STOPIE | I2C_CR1_NACKIE;

  if (0 == (I2C1->CR1 & I2C_CR1_PE))
    I2C1->CR1 |= I2C_CR1_PE;
}