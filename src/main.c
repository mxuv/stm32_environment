#include <stdint.h>

#include "hal.h"
#include "gpio.h"
#include "gpio_config.h"
#include "i2c_hal.h"
#include "delay.h"
#include "os.h"

#include "hd44780_i2c.h"
#include "nconv.h"
#include "str.h"
#include "mdiv.h"

uint32_t uptime = 1000;

void print_uptime(void)
{
    char str[11];
    const char *s;
    uint32_t t;

    hd44780_i2c_setcursor(0, 1);
    hd44780_i2c_sendstring("Uptime:");

    GPIO_PIN_SET(TEST_PIN_PORT, TEST_PIN);
    uptime = uptime / 100;
    GPIO_PIN_RESET(TEST_PIN_PORT, TEST_PIN);

    uint_str(sizeof(uptime), uptime, str);
    s = skipzeros(str);

    hd44780_i2c_sendstring(s);
    hd44780_i2c_sendchar('s');
    hd44780_i2c_refresh();

    uptime += 1010;

    os_set_timer_task(print_uptime, 1000);
}

void print_test(void)
{
    hd44780_i2c_setcursor(5, 0);
    hd44780_i2c_sendstring("Test mode");
    hd44780_i2c_refresh();
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

    os_set_timer_task(print_test, 100);
    os_set_timer_task(print_uptime, 1000);
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
