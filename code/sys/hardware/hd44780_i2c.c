#include <stdint.h>

#include "hd44780_i2c.h"
#include "hd44780.h"
#include "delay.h"
#include "i2c.h"
#include "os.h"

#ifdef HD44780_I2C_EN

uint8_t HD44780_I2C_State = 0;

void hd44780_i2c_Init(void)
{
  i2c_master_buffer_index = 0;
  i2c_master_nbytes = 0;

  i2c_master_buffer_push(HD44780_8B_2L_5X8 | HD44780_I2C_E); //8-ми битный порт, 2 строки, шрифт 5х7
  i2c_master_buffer_push(HD44780_8B_2L_5X8 & ~(HD44780_I2C_E));

  i2c_master_buffer_push(HD44780_4B_2L_5X8 | HD44780_I2C_E); //4-ми битный порт, 2 строки, шрифт 5х7
  i2c_master_buffer_push(HD44780_4B_2L_5X8 & ~(HD44780_I2C_E));

  hd44780_i2c_Tx(HD44780_4B_2L_5X8, HD44780_TX_COM); //4-ми битный порт, 2 строки, шрифт 5х7
  hd44780_i2c_Tx(HD44780_DISPLAY_OFF, HD44780_TX_COM);            //выкл дисплей
  hd44780_i2c_Tx(HD44780_DISPLAY_CLEAR, HD44780_TX_COM);      //очищение дисплея

  HD44780_I2C_State |= (HD44780_I2C_Processed | HD44780_I2C_Init1);

  i2c_masterdone = hd44780_i2c_InitNxt;
  i2c_slaveaddr = HD44780_I2C_ADDR;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

void hd44780_i2c_Tx(uint8_t data, uint8_t EnDat)
{
  uint8_t tmp = data & ~(HD44780_I2C_PMSK);			 //Сначала передаем старшие биты
  if (EnDat)									           //если передаем данные,то выставляем RS
    tmp |= HD44780_I2C_RS;
  if (HD44780_I2C_State & HD44780_I2C_LightEn) //Если бит подсветки выставлен, то включаем подсветку
    tmp |= HD44780_I2C_Light_PIN;
  i2c_master_buffer_push(tmp | HD44780_I2C_E);
  i2c_master_buffer_push(tmp & ~(HD44780_I2C_E));
  tmp = (data & HD44780_I2C_PMSK) << 4;			            //Теперь младшие
  if (EnDat)									           //если передаем данные,то выставляем RS
    tmp |= HD44780_I2C_RS;
  if (HD44780_I2C_State & HD44780_I2C_LightEn) //Если бит подсветки выставлен, то включаем подсветку
    tmp |= HD44780_I2C_Light_PIN;
  i2c_master_buffer_push(tmp | HD44780_I2C_E);
  i2c_master_buffer_push(tmp & ~(HD44780_I2C_E));
}

void hd44780_i2c_Clear(void)
{
  hd44780_i2c_Tx(HD44780_DISPLAY_CLEAR, HD44780_TX_COM);
}

void hd44780_i2c_SendChar(uint8_t ch)
{
  hd44780_i2c_Tx(ch, HD44780_TX_DAT);
}

void hd44780_i2c_SendStringC(uint8_t f_ch, uint8_t n_ch, const char *str)
{
  str+= f_ch;
  while (n_ch)
  {
    hd44780_i2c_SendChar(*str);
    str++;
    n_ch--;
  }
}


void hd44780_i2c_SendString(const char *str)
{
  while (*str != '\0')
  {
    hd44780_i2c_SendChar(*str);
    str++;
  }
}

void hd44780_i2c_InitNxt(void)
{
  HD44780_I2C_State |= HD44780_I2C_Init2 | HD44780_I2C_Processed;
  _delay_ms(2);

  i2c_master_buffer_index = 0;
  i2c_master_nbytes = 0;

  HD44780_I2C_State |= HD44780_I2C_LightEn;                			//Вкл подсветки

  hd44780_i2c_Tx(HD44780_CUR_SHIFT_EN_R, HD44780_TX_COM); //разрешаем сдвиг курсора вправо, сдвиг экрана запрещен
  hd44780_i2c_Tx(HD44780_DISPLAY_ON, HD44780_TX_COM);       //включение дисплея

  i2c_masterdone = hd44780_i2c_Ok;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

void hd44780_i2c_Ok(void)
{
  HD44780_I2C_State &= HD44780_I2C_LightMsk;
  i2c_master_reset_state();
}

void hd44780_i2c_Refresh(void)
{
  i2c_slaveaddr = HD44780_I2C_ADDR;
  i2c_masterdone = hd44780_i2c_Ok;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

void hd44780_i2c_SetCursor(uint8_t X, uint8_t Y)
{
  hd44780_i2c_Tx(X + HD44780_DDRAM + (Y * 0x40), 0);
}

void hd44780_i2c_LightSw(void)
{
  if (HD44780_I2C_State & HD44780_I2C_LightEn)
    i2c_master_buffer[0] |= HD44780_I2C_Light_PIN;
  else
    i2c_master_buffer[0] &= ~(HD44780_I2C_Light_PIN);
  i2c_master_nbytes = 1;
  i2c_master_buffer_index = 0;
  i2c_masterdone = hd44780_i2c_Ok;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;
  i2c_start();
}

void HD44780_I2C_Light(void)
{
  if (i2c_state & I2C_BUS_BUSY)
  {                   //Если шина занята попробуем попозже
    os_set_timer_task(HD44780_I2C_Light, 10);
    return;
  }
  i2c_master_buffer_index = 0;
  i2c_master_nbytes = 1;

  i2c_slaveaddr = HD44780_I2C_ADDR;
  i2c_masterdone = hd44780_i2c_LightSw;
  i2c_state = I2C_MODE_SR | I2C_BUS_BUSY; //занимаем шину и говорим прочитать данные из порта
  i2c_start();
}

void hd44780_i2c_LightOn(void)
{
  HD44780_I2C_State |= HD44780_I2C_LightEn;          //Выставляем флаг подсветки
  HD44780_I2C_Light();
}

void hd44780_i2c_LightOff(void)
{
  HD44780_I2C_State &= ~HD44780_I2C_LightEn;
  HD44780_I2C_Light();
}

#endif
