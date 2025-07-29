#ifndef __I2C_H__
#define __I2C_H__
#include "hw_config.h"
#include "i2c_config.h"
#include "nstdtypes.h"

#ifdef I2C_EN
#ifdef I2C_SUPERVISOR_EN
#include "os.h"
#endif

#define I2C_MODE_MSK            0b0000000111000000 /*Маска режимов работы*/
#define I2C_MODE_SW            	0b0000000000000000 /*Режим работы Start+Adr+W+Write */
#define I2C_MODE_SR            	0b0000000001000000 /*Режим работы Start+Adr+R+Read */
#define I2C_MODE_SWSW         	0b0000000010000000 /*Режим работы Start+Adr+W+Write+Start+W+Write */
#define I2C_MODE_SWSR         	0b0000000011000000 /*Режим работы Start+Adr+W+Write+Start+R+Read */

#define I2C_STARTED							0b0000000000100000 //Был старт
#define I2C_WRITE_MEM_ADDR			0b0000001000000000 //Пишем адрес памяти
#define I2C_MASTER_WORKING			0b0000010000000000 //Мастер работает
#define I2C_SLAVE_WORKING				0b0000100000000000 //Слэйв работает
#define I2C_SLAVE_RECEIVED      0b0100000000000000 //Слэйв принял данные
#define I2C_MASTER_INTERRUPTED	0b0001000000000000 //Мастера прервали

#define I2C_BUS_BUSY            0b0000000000010000 //Шина занята
#define I2C_BUS_FREE            0b1111111111101111 //Шина свободна

#define I2C_ERR_MSK             0b0010000000001111 //Маска кодов ошибок
#define I2C_ERR_OK              0b0000000000000000 //Ошибок нет, все ОК
#define I2C_ERR_NA              0b0000000000000001 //Слейв не отвечает на свой адрес
#define I2C_ERR_NK              0b0000000000000010 //Передали байт, а получили NACK
#define I2C_ERR_BF              0b0000000000000100 //Ошибка на шине
#define I2C_ERR_EP              0b0000000000001000 //Ошибка приоритета, кто-то встал на шину поглавнее
#define I2C_ERR_SU              0b0010000000000000 //Отработал супервизор

#define I2C_BUS_ISBUSY					(i2c_state & I2C_BUS_BUSY)
#define I2C_ERROR								(i2c_state & I2C_ERR_MSK)

extern uint16_t i2c_state;
extern uint8_t i2c_slaveaddr;
extern uint8_t i2c_membuf[I2C_MEM_ADDRSIZE];
extern uint8_t i2c_membuf_index;
extern uint8_t i2c_membuf_nbytes;
extern uint8_t i2c_masterbuf_index;
extern uint8_t i2c_masterbuf[I2C_MASTER_BUFSIZE];

#ifdef I2C_LONG_MODE_EN
extern uint16_t i2c_master_nbytes;
#else
extern uint8_t i2c_master_nbytes;
#endif

extern uint8_t i2c_slave_outbuf[I2C_SLAVE_OUT_BUFSIZE];
extern uint8_t i2c_slave_inbuf[I2C_SLAVE_IN_BUFSIZE];

extern fptr_t i2c_masterdone;
extern fptr_t i2c_slavedone;
extern fptr_t i2c_softerror;
extern fptr_t i2c_harderror;

void i2c_start(void);
void i2c_master_reset_state(void);
void i2c_master_reset_index(void);
void i2c_slave_done_default(void);
uint8_t i2c_master_buffer_push(uint8_t data);

#ifdef I2C_SUPERVISOR_EN
void i2c_Supervisor(void);
#endif

void i2c_void(void);

#endif
#endif
