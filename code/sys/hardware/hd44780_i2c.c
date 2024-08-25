#include <stdint.h>

#include "hd44780_i2c.h"
#include "hd44780.h"
#include "delay.h"
#include "i2c.h"
#include "os.h"

#ifdef HD44780_I2C_EN

static void hd44780_i2c_ok(void);
static void hd44780_i2c_init2(void);
static void hd44780_i2c_init3(void);
static void hd44780_i2c_error(void);
static void hd44780_i2c_lightsw(void);
static void hd44780_i2c_initnxt(void);
static void hd44780_i2c_tx(uint8_t data, uint8_t endat);

static uint8_t hd44780_i2c_state = 0;

void hd44780_i2c_init(void)
{
  i2c_master_buffer_index = 0;
  i2c_master_nbytes = 0;

#ifdef HD44780_I2C_DELAY_BEFORE_INIT_EN
  _delay_ms(HD44780_I2C_DELAY_BI_TIME);
#endif
  /* 8-bit port, 2 strings, font 5x7 */
  i2c_master_buffer_push(HD44780_8B_2L_5X8 | HD44780_I2C_E);
  i2c_master_buffer_push(HD44780_8B_2L_5X8 & ~(HD44780_I2C_E));

  hd44780_i2c_state |= (HD44780_I2C_PROCESSED | HD44780_I2C_INIT1);

  i2c_masterdone = hd44780_i2c_init2;
  i2c_slaveaddr = HD44780_I2C_ADDR;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

static void hd44780_i2c_init2(void)
{
  i2c_master_buffer_index = 0;
  i2c_master_nbytes = 0;

  /* 4-bit port, 2 strings, font 5x7 */
  i2c_master_buffer_push(HD44780_4B_2L_5X8 | HD44780_I2C_E);
  i2c_master_buffer_push(HD44780_4B_2L_5X8 & ~(HD44780_I2C_E));

  i2c_masterdone = hd44780_i2c_init3;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;

  os_set_timer_task(i2c_start, 5);
}

static void hd44780_i2c_init3(void)
{
  i2c_master_buffer_index = 0;
  i2c_master_nbytes = 0;

  /* 4-bit port, 2 strings, font 5x7 */
  hd44780_i2c_tx(HD44780_4B_2L_5X8, HD44780_TX_COM);            /* 4-bit port, 2 strings, font 5x7 */
  hd44780_i2c_tx(HD44780_DISPLAY_OFF, HD44780_TX_COM);          /* display off */
  hd44780_i2c_tx(HD44780_DISPLAY_CLEAR, HD44780_TX_COM);        /* display clear */

  i2c_masterdone = hd44780_i2c_initnxt;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;

  os_set_timer_task(i2c_start, 5);
}

static void hd44780_i2c_tx(uint8_t data, uint8_t endat)
{
  uint8_t tmp = data & ~(HD44780_I2C_PMSK);                     /* major bits first */

  if (endat)                                                    /* if data tx >> RS en */
    tmp |= HD44780_I2C_RS;
  if (hd44780_i2c_state & HD44780_I2C_LIGHT_EN)                 /* if backlight enable set bit */
    tmp |= HD44780_I2C_LIGHT_PIN;
  i2c_master_buffer_push(tmp | HD44780_I2C_E);
  i2c_master_buffer_push(tmp & ~(HD44780_I2C_E));
  tmp = (data & HD44780_I2C_PMSK) << 4;                         /* least bits */
  if (endat)                                                    /* if data tx >> RS en */
    tmp |= HD44780_I2C_RS;
  if (hd44780_i2c_state & HD44780_I2C_LIGHT_EN)                 /* if backlight enable set bit */
    tmp |= HD44780_I2C_LIGHT_PIN;
  i2c_master_buffer_push(tmp | HD44780_I2C_E);
  i2c_master_buffer_push(tmp & ~(HD44780_I2C_E));
}

void hd44780_i2c_clear(void)
{
  hd44780_i2c_tx(HD44780_DISPLAY_CLEAR, HD44780_TX_COM);
}

void hd44780_i2c_sendchar(uint8_t ch)
{
  hd44780_i2c_tx(ch, HD44780_TX_DAT);
}

void hd44780_i2c_sendstringc(uint8_t f_ch, uint8_t n_ch, const char *str)
{
  str+= f_ch;
  while (n_ch)
  {
    hd44780_i2c_sendchar(*str);
    str++;
    n_ch--;
  }
}

void hd44780_i2c_sendstring(const char *str)
{
  while (*str != '\0')
  {
    hd44780_i2c_sendchar(*str);
    str++;
  }
}

static void hd44780_i2c_initnxt(void)
{
  hd44780_i2c_state |= HD44780_I2C_INIT2 | HD44780_I2C_PROCESSED;
  _delay_ms(2);

  i2c_master_buffer_index = 0;
  i2c_master_nbytes = 0;

  hd44780_i2c_state |= HD44780_I2C_LIGHT_EN;                      /* backlight on */

  hd44780_i2c_tx(HD44780_CUR_SHIFT_EN_R, HD44780_TX_COM);         /* cursor shifting enable, display shifting disable */
  hd44780_i2c_tx(HD44780_DISPLAY_ON, HD44780_TX_COM);             /* display on */

  i2c_masterdone = hd44780_i2c_ok;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

static void hd44780_i2c_errordone(void)
{
  hd44780_i2c_state &= HD44780_I2C_LIGHT_MSK;
  os_set_timer_task(i2c_master_reset_state, 5);
}

static void hd44780_i2c_error(void)
{
  hd44780_i2c_state &= HD44780_I2C_LIGHT_MSK;
  i2c_master_reset_state();
  hd44780_i2c_clear();

  i2c_masterdone = hd44780_i2c_errordone;
  i2c_softerror = hd44780_i2c_error;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

static void hd44780_i2c_ok(void)
{
  hd44780_i2c_state &= HD44780_I2C_LIGHT_MSK;
  i2c_master_reset_state();
}

void hd44780_i2c_refresh(void)
{
  i2c_slaveaddr = HD44780_I2C_ADDR;
  i2c_masterdone = hd44780_i2c_ok;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

/* DDRAM Address for 4-string display:
 * 1st string - 0x00
 * 2st string - 0x40
 * 3st string - 0x14
 * 4st string - 0x54
 */
void hd44780_i2c_setcursor(uint8_t x, uint8_t y)
{
#if defined(HD44780_I2C_4S) && defined(HD44780_I2C_2S)
#error Display string mode collision
#endif

#if !defined(HD44780_I2C_4S) && !defined(HD44780_I2C_2S)
#error No selected string mode
#endif

#ifdef HD44780_I2C_4S
  switch (y)
  {
  case 0:
    y = 0;
    break;
  case 1:
    y = 0x40;
    break;
  case 2:
    y = 0x14;
    break;
  case 3:
    y = 0x54;
    break;
  default:
    break;
  }
  hd44780_i2c_tx(x + HD44780_DDRAM + y, HD44780_TX_COM);
#endif
#ifdef HD44780_I2C_2S
  hd44780_i2c_tx(x + HD44780_DDRAM + (y * 0x40), HD44780_TX_COM);
#endif
}

static void hd44780_i2c_lightsw(void)
{
  if (hd44780_i2c_state & HD44780_I2C_LIGHT_EN)
    i2c_master_buffer[0] |= HD44780_I2C_LIGHT_PIN;
  else
    i2c_master_buffer[0] &= ~(HD44780_I2C_LIGHT_PIN);
  i2c_master_nbytes = 1;
  i2c_master_buffer_index = 0;
  i2c_masterdone = hd44780_i2c_ok;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

static void hd44780_i2c_light(void)
{
  if (i2c_state & I2C_BUS_BUSY)
  {
    os_set_timer_task(hd44780_i2c_light, 10);
    return;
  }
  i2c_master_buffer_index = 0;
  i2c_master_nbytes = 1;

  i2c_slaveaddr = HD44780_I2C_ADDR;
  i2c_masterdone = hd44780_i2c_lightsw;
  i2c_state = I2C_MODE_SR | I2C_BUS_BUSY;
  i2c_start();
}

void hd44780_i2c_lighton(void)
{
  hd44780_i2c_state |= HD44780_I2C_LIGHT_EN;
  hd44780_i2c_light();
}

void hd44780_i2c_lightoff(void)
{
  hd44780_i2c_state &= ~HD44780_I2C_LIGHT_EN;
  hd44780_i2c_light();
}

#endif
