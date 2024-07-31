#include <stdint.h>

#include "ds18b20.h"
#include "one_wire.h"
#include "nconv.h"
#include "nstdtypes.h"

#ifdef DS18B20_EN

#if defined(DS18B20_SINGLEMODE) && defined(DS18B20_MULTIMODE)
#error DS18B20 mode collision
#endif

#define CRC8INIT   0x00
#define CRC8POLY   0x18              /*0X18 = X^8+X^5+X^4+X^0*/

#ifdef DS18B20_SINGLEMODE
static int16_t ds18b20_temperature;

int16_t ds18b20_get_current_temperature(void)
{
  return ds18b20_temperature;
}

void ds18b20_read_temperature(void)
{
  onewire_reset();
  onewire_txbyte(DS18B20_SKIP_ROM);
  onewire_txbyte(DS18B20_READ_SCRATCHPAD);
  onewire_buffer[0] = onewire_rxbyte();
  onewire_buffer[1] = onewire_rxbyte();
  onewire_reset();
  ds18b20_result_conversion();
}

void ds18b20_conversion_temperature(void)
{
  onewire_reset();
  onewire_txbyte(DS18B20_SKIP_ROM);
  onewire_txbyte(DS18B20_CONVERT_T);
}

void ds18b20_result_conversion(void)
{
  int32_t buf_temp = (int16_t)(onewire_buffer[0] | (onewire_buffer[1] << 8));
  int32_t sub_num = 1000000, term_num;

  ds18b20_temperature = 0;

  if (buf_temp < 0)
  {
    buf_temp = buf_temp * -1;
    term_num = -1000;
  }
  else
    term_num = 1000;

  buf_temp = buf_temp * DS18B20_MUL;

  for (uint8_t i = 0; i < 4; i++)
  {
    while (buf_temp >= sub_num)
    {
      ds18b20_temperature = ds18b20_temperature + term_num;
      buf_temp = buf_temp - sub_num;
    }
    term_num = term_num / 10;
    sub_num = sub_num / 10;
  }
}
#endif

#ifdef DS18B20_MULTIMODE
static uint64_t ds18b20_rom[DS18B20_NSENSORS];
static int16_t ds18b20_temperature[DS18B20_NSENSORS];

int16_t ds18b20_get_current_temperature(uint8_t sensor)
{
  return ds18b20_temperature[sensor];
}

int16_t ds18b20_get_max_value(void)
{
  int16_t temp = DS18B20_MIN_VAL;
  uint8_t i;

  for (i = 0; i < DS18B20_NSENSORS; i++)
  {
    if (ds18b20_temperature[i] > temp)
      temp = ds18b20_temperature[i];
  }
  return temp;
}

void ds18b20_match_rom(uint8_t sensor)
{
  num64_t temp_rom = {ds18b20_rom[sensor]};

  onewire_txbyte(DS18B20_MATCH_ROM);
  for (uint8_t i = 0; i < sizeof(temp_rom); i++)
    onewire_txbyte(temp_rom.byte[i]);
}

void ds18b20_read_scrathpad(uint8_t sensor)
{
  onewire_reset();
  ds18b20_match_rom(sensor);
  onewire_txbyte(DS18B20_READ_SCRATCHPAD);
  for (uint8_t i = 0; i < 9; i++)
    onewire_buffer[i] = onewire_rxbyte();
}

void ds18b20_read_temperature(uint8_t sensor)
{
  onewire_reset();
  ds18b20_match_rom(sensor);
  onewire_txbyte(DS18B20_READ_SCRATCHPAD);
  onewire_buffer[0] = onewire_rxbyte();
  onewire_buffer[1] = onewire_rxbyte();
  onewire_reset();
  ds18b20_result_conversion(sensor);
}

void ds18b20_conversion_temperature(uint8_t sensor)
{
  onewire_reset();
  ds18b20_match_rom(sensor);
  onewire_txbyte(DS18B20_CONVERT_T);
}

void ds18b20_conversion_temperature_all(void)
{
  onewire_reset();
  onewire_txbyte(DS18B20_SKIP_ROM);
  onewire_txbyte(DS18B20_CONVERT_T);
}

void ds18b20_search_rom(void)
{
  uint64_t *current_rom = ds18b20_rom;
  uint64_t branch_flag = 0;
  uint64_t temp_rom = 0;
  uint64_t current_bit;
  uint8_t received_bit;
  for (uint8_t j = 0; j < DS18B20_NSENSORS; j++)
  {
    current_bit = 1;
    temp_rom = 0;
    onewire_reset();                                        /*Сброс*/
    onewire_txbyte(DS18B20_SEARCH_ROM);                     /*Поиск*/
    for (uint8_t i = 0; i < 64; i++)
    {
      received_bit = (onewire_rx() << 1) | onewire_rx();   /*Смотрим какой бит к нам пришел*/
      if (received_bit == 1)                                 /*01*/
        onewire_tx0();

      if (received_bit == 2)                                 /*10*/
      {
        temp_rom |= current_bit;
        onewire_tx1();
      }
      if (received_bit == 3)                                 /*3 - Устройства не обнаружены*/
      {
        *current_rom = 0;
        break;
      }
      if (received_bit == 0)                                 /*00 - коллизия номеров*/
      {
        if (current_bit & branch_flag)
        {
          if ((branch_flag & ~current_bit) > current_bit)
          {
            branch_flag |= current_bit;
            onewire_tx0();
          }
          else
          {
            branch_flag &= ~current_bit;
            temp_rom |= current_bit;
            onewire_tx1();
          }
        }
        else
        {
          if ((branch_flag & ~current_bit) > current_bit)
          {
            branch_flag &= ~current_bit;
            temp_rom |= current_bit;
            onewire_tx1();
          }
          else
          {
            branch_flag |= current_bit;
            onewire_tx0();
          }
        }
      }
      current_bit = current_bit << 1;
    }
    *current_rom = temp_rom;
    current_rom++;
  }
}

void ds18b20_result_conversion(uint8_t sensor)
{
  int32_t buf_temp = (int16_t)(onewire_buffer[0] | (onewire_buffer[1] << 8));
  int32_t sub_num = 1000000, term_num;

  ds18b20_temperature[sensor] = 0;

  if (buf_temp < 0)
  {
    buf_temp = buf_temp * -1;
    term_num = -1000;
  }
  else
    term_num = 1000;

  buf_temp = buf_temp * DS18B20_MUL;

  for (uint8_t i = 0; i < 4; i++)
  {
    while (buf_temp >= sub_num)
    {
      ds18b20_temperature[sensor] += term_num;
      buf_temp -= sub_num;
    }
    term_num = term_num / 10;
    sub_num = sub_num / 10;
  }
}
#endif

void ds18b20_temp2string(uint8_t *ch, int16_t sensor_temperature)
{
	uint32_t val_tmp, rank = 1000;
	uint8_t *ptemp = ch;

	if (sensor_temperature < 0)
	{
		val_tmp = sensor_temperature * -1;
		*ch = '-';
		ch++;
	}
	else
	  val_tmp = sensor_temperature;

	for (uint8_t i = 0; i < 3; i++)
	{
	  if (val_tmp >= rank || ch != ptemp)
	  {
	    *ch = mrank(&val_tmp, rank) + 0x30;
	    ch++;
	  }
	  rank = rank / 10;
	}

  *ch = '.';
  ch++;
  *ch = mrank(&val_tmp, rank) + 0x30;
}


#ifdef DS18B20_CRCEN
uint8_t ds18b20_crc(uint8_t data, uint8_t crc)
{
  uint8_t bit_counter = 8;
  uint8_t feedback_bit;

  do
  {
    feedback_bit = (crc ^ data) & 0x01;
    if (feedback_bit == 0x01)
      crc = crc ^ CRC8POLY;
    crc = (crc >> 1) & 0x7F;
    if (feedback_bit == 0x01)
      crc = crc | 0x80;
    data = data >> 1;
    bit_counter--;
  } while (bit_counter > 0);

  return crc;
}

uint8_t ds18b20_crccalc(uint8_t *data, uint8_t nbytes)
{
  uint8_t i, crc = 0;

  for (i = 0; i < nbytes; i++)
  {
    crc = ds18b20_crc(*data, crc);
    data++;
  }
  return crc;
}

uint8_t ds18b20_crcscratch(void)
{
  return ds18b20_crccalc(&onewire_buffer[0], 9);
}
#endif

#if defined(DS18B20_MULTIMODE) && defined(DS18B20_CRCEN)
uint8_t ds18b20_crcrom(uint8_t sensor)
{
  return ds18b20_crccalc((uint8_t*)&ds18b20_rom[sensor], 8);
}
#endif

#endif
