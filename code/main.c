#include <stdint.h>

#include "hal.h"
#include "os.h"

int main(void)
{
  systemclock_init();
  hardware_init();
  systick_init();
  os_init();

  for (;;)
  {
    os_task_manager();
  }
}

void SysTick_Handler(void)
{
  os_timer_service();
}
