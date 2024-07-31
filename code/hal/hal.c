#include "hal.h"
#include "mcu.h"
#include "rcc.h"
#include "tim.h"
#include "flash.h"
#include "gpio.h"
#include "hal_config.h"
#include "gpio_config.h"
#include "system.h"
#include "exti.h"

void systemclock_init(void)
{
  // Set fast FLASH speed
  FLASH_SET_LATENSY_1();
  if ((FLASH->ACR & FLASH_LATENCY_1) == 0)
  {
    device_error();
  }

  //HSE Enable 8 MHz
  /*HSE_ON();
   while (HSE_RDY == 0)
   {

   }*/

  //Set SYSCLK clock 48 MHz
  PLL_SET_SRC(PLL_SRC_HSI_DIV2);
  PLL_SET_MUL(PLL_MUL_12);
  PLL_ON();
  while (PLL_RDY == 0)
  {

  }

  //Set AHB clock 48 MHz
  AHB_SET_PRESCALER(HCLK_DIV_1);

  //Set APB clock 48 MHz
  APB_SET_PRESCALER(PCLK_DIV_1);

  //PLL selected as system clock
  SYSCLK_SET_SRC(SYSCLK_SRC_PLL);
  while (SYSCLK_SWS != SYSCLK_SWS_PLL)
  {

  }

  sysclock = 48000000;
  RCC_I2C1_CLK_SRC_SYSCLK();
  RCC_USART1_CLK_SRC(RCC_USART1_CLK_SRC_SYSCLK);

  HSI_14_ON();
  while (HSI_14_RDY == 0)
  {

  }
}

void hardware_init(void)
{

}

void systick_init(void)
{
  SysTick_Config(sysclock / 1000 - 1);//Запуск системного таймера с периодом 1мс
  NVIC_EnableIRQ(SysTick_IRQn);
}
