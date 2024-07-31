#include <stdint.h>

#include "mcu.h"
#include "delay.h"

void _delay_ms(uint32_t Delay)
{
  __IO uint32_t tmp = SysTick->CTRL; /* Clear the COUNTFLAG first */
  /* Add this code to indicate that local variable is not used */
  ((void) tmp);

  /*Add a period to guaranty minimum wait */
  if (Delay < 0xFFFFFFFFU)
  {
    Delay++;
  }

  while (Delay)
  {
    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
      Delay--;
    }
  }
}
