#include <stdint.h>

#include "hal.h"
#include "gpio.h"
#include "gpio_config.h"
#include "i2c_hal.h"
#include "delay.h"
#include "os.h"

#include "hd44780_i2c.h"

void display_string(void)
{
    hd44780_i2c_setcursor(0, 0);
    hd44780_i2c_sendstring("Hello");
    hd44780_i2c_setcursor(0, 1);
    hd44780_i2c_sendstring("From");
    hd44780_i2c_setcursor(0, 2);
    hd44780_i2c_sendstring("4 string");
    hd44780_i2c_setcursor(0, 3);
    hd44780_i2c_sendstring("Display");
    hd44780_i2c_refresh();
    os_set_timer_task(display_string, 500);
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

int main(void)
{
    systemclock_init();
    hardware_init();
    systick_init();
    i2c_init();
    os_init();

    _delay_ms(500);
    hd44780_i2c_init();
    _delay_ms(100);

    os_set_timer_task(display_string, 100);
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
