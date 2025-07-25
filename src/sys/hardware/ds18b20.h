#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "ds18b20_config.h"

#define DS18B20_READ_ROM						0x33
#define DS18B20_MATCH_ROM						0x55
#define DS18B20_SKIP_ROM						0xCC
#define DS18B20_ALARM_SEARCH				0xEC
#define DS18B20_SEARCH_ROM					0xF0

#define DS18B20_WRITE_SCRATCHPAD		0x4E
#define DS18B20_READ_SCRATCHPAD			0xBE
#define DS18B20_COPY_SCRATCHPAD			0x48
#define DS18B20_CONVERT_T						0x44
#define DS18B20_RECALL_E2						0xB8
#define DS18B20_READ_POWEWR_SUPPLY	0xB4

#define DS18B20_TR_9BIT							0b00011111
#define DS18B20_TR_10BIT						0b00111111
#define DS18B20_TR_11BIT						0b01011111
#define DS18B20_TR_12BIT						0b01111111


#define DS18B20_TIMECONV_9BIT       100
#define DS18B20_TIMECONV_10BIT      192
#define DS18B20_TIMECONV_11BIT      400
#define DS18B20_TIMECONV_12BIT      755

#define DS18B20_MUL									625
#define DS18B20_NEG_F								0x80

#define DS18B20_MAX_VAL             1250
#define DS18B20_MIN_VAL             -550

#ifdef DS18B20_SINGLEMODE
void ds18b20_read_temperature(void);
void ds18b20_result_conversion(void);
void ds18b20_conversion_temperature(void);
int16_t ds18b20_get_current_temperature(void);
#endif

#ifdef DS18B20_MULTIMODE
int16_t ds18b20_get_max_value(void);
int16_t ds18b20_get_current_temperature(uint8_t sensor);
void ds18b20_read_temperature(uint8_t sensor);
void ds18b20_read_scrathpad(uint8_t sensor);
void ds18b20_conversion_temperature(uint8_t sensor);
void ds18b20_conversion_temperature_all(void);
void ds18b20_search_rom(void);
void ds18b20_result_conversion(uint8_t sensor);
#endif

#ifdef DS18B20_CRCEN
uint8_t ds18b20_crc(uint8_t data, uint8_t crc);
uint8_t ds18b20_crccalc(uint8_t *data, uint8_t nbytes);
uint8_t ds18b20_crcscratch(void);
#endif

#if defined(DS18B20_MULTIMODE) && defined(DS18B20_CRCEN)
uint8_t ds18b20_crcrom(uint8_t sensor);
#endif

void ds18b20_temp2string(uint8_t *ch, int16_t sensor_temperature);

#endif
