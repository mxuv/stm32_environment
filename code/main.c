#include <stdint.h>

#include "hal.h"
#include "i2c_hal.h"
#include "delay.h"
#include "os.h"

#include "hd44780_i2c.h"

void display_string(void)
{
  hd44780_i2c_setcursor(0, 1);
  hd44780_i2c_sendstring("0123456789ABCDEFGHJKLMNOPQRSTUVW");
  hd44780_i2c_refresh();
}

int main(void)
{
  systemclock_init();
  hardware_init();
  systick_init();
  i2c_init();
  os_init();

  _delay_ms(500);
  hd44780_i2c_init();

  os_set_timer_task(display_string, 500);

  for (;;)
  {
    os_task_manager();
  }
}

void SysTick_Handler(void)
{
  os_timer_service();
}
