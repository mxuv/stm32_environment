#include <stdint.h>

#include "hal.h"
#include "gpio.h"
#include "gpio_config.h"
#include "i2c_hal.h"
#include "delay.h"
#include "os.h"

#include "str.h"
#include "mdiv.h"

#include "hd44780_i2c.h"
#include "one_wire.h"
#include "ds18b20.h"

void print_temperature(void)
{
  char str[11];
  ds18b20_temp2string(str, ds18b20_get_current_temperature(0));
  hd44780_i2c_setcursor(0, 1);
  hd44780_i2c_sendstring("T1 = ");
  hd44780_i2c_sendstring(str);
  hd44780_i2c_sendchar(' ');
  ds18b20_temp2string(str, ds18b20_get_current_temperature(1));
  hd44780_i2c_sendstring("T2 = ");
  hd44780_i2c_sendstring(str);
  hd44780_i2c_sendchar(' ');
  hd44780_i2c_refresh();
  os_set_timer_task(print_temperature, 800);
}

void scan_red_btn(void)
{
    if (GPIO_GET_PIN(BTN_RED_PORT, BTN_RED_PIN))
        GPIO_PIN_RESET(LED_PORT, LED_RED_PIN);
    else
        GPIO_PIN_SET(LED_PORT, LED_RED_PIN);

    os_set_task(scan_red_btn);
}

void scan_grn_btn(void)
{
    if (GPIO_GET_PIN(BTN_GRN_PORT, BTN_GRN_PIN))
        GPIO_PIN_RESET(LED_PORT, LED_GRN_PIN);
    else
        GPIO_PIN_SET(LED_PORT, LED_GRN_PIN);
    os_set_task(scan_grn_btn);
}

void ts1_get_temp(void);

void ts_init(void)
{
  DISABLE_INTERRUPTS();
  ds18b20_search_rom();
  ENABLE_INTERRUPTS();

  if (ds18b20_crcrom(0)) {
    hd44780_i2c_setcursor(0, 0);
    hd44780_i2c_sendstring("TS1 fail");
  }

  if (ds18b20_crcrom(1)) {
    hd44780_i2c_setcursor(0, 10);
    hd44780_i2c_sendstring("TS2 fail");
  }
  hd44780_i2c_refresh();
  os_set_task(ts1_get_temp);
}

void ts2_read_data(void)
{
  DISABLE_INTERRUPTS();
  ds18b20_read_scrathpad(1);
  ENABLE_INTERRUPTS();
  ds18b20_result_conversion(1);
  os_set_task(ts1_get_temp);
}

void ts1_read_data(void)
{
  DISABLE_INTERRUPTS();
  ds18b20_read_scrathpad(0);
  ENABLE_INTERRUPTS();
  GPIO_PIN_SET(TEST_PIN_PORT, TEST_PIN);
  ds18b20_result_conversion(0);
  GPIO_PIN_RESET(TEST_PIN_PORT, TEST_PIN);
  os_set_task(ts2_read_data);
}

void ts2_get_temp(void)
{
  DISABLE_INTERRUPTS();
  ds18b20_conversion_temperature(1);
  ENABLE_INTERRUPTS();
  os_set_timer_task(ts1_read_data, 755);
}

void ts1_get_temp(void)
{
  DISABLE_INTERRUPTS();
  ds18b20_conversion_temperature(0);
  ENABLE_INTERRUPTS();
  os_set_timer_task(ts2_get_temp, 20);
}
void onewire_checkbus(void)
{
  int t;

  t = onewire_reset();
  if (t & OW_ERR_MSK) {
    hd44780_i2c_setcursor(0, 0);
    hd44780_i2c_sendstring("OW BUS ERROR");
    hd44780_i2c_refresh();
  } else {
    ts_init();
    os_set_timer_task(print_temperature, 1000);
  }
}

int main(void)
{
    systemclock_init();
    hardware_init();
    systick_init();
    i2c_init();

    os_init();

    onewire_init();
    onewire_reset();

    _delay_ms(10);
    hd44780_i2c_init();

    os_set_timer_task(onewire_checkbus, 500);
    os_set_task(scan_red_btn);
    os_set_task(scan_grn_btn);

    for (;;) {
        os_task_manager();
    }
}

void SysTick_Handler(void)
{
    os_timer_service();
}
