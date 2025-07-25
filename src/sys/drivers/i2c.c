#include <stdint.h>

#include "i2c.h"
#include "i2c_hal.h"

#ifdef I2C_EN
uint16_t i2c_state = 0;                  							/* Переменная состояния IIC */
uint8_t i2c_slaveaddr;                                /* Адрес слейва */

uint8_t i2c_membuffer[I2C_MEM_ADDRSIZE];              /* Адрес памяти */
uint8_t i2c_membuffer_index = 0;
uint8_t i2c_membuffer_nbytes = 0;

uint8_t i2c_master_buffer_index = 0;
uint8_t i2c_master_nbytes = 0;
uint8_t i2c_master_buffer[I2C_MASTER_BUFSIZE];        /* Буфер в режиме мастера */


uint8_t i2c_slave_inbufindex = 0;
uint8_t i2c_slave_inbuffer[I2C_SLAVE_IN_BUFSIZE];     /* Буфер в режиме слейва на прием */
uint8_t i2c_slave_outbuffer_index = 0;
uint8_t i2c_slave_outbuffer[I2C_SLAVE_OUT_BUFSIZE];   /* Буфер в режиме мастера на передачу */

fptr_t i2c_masterdone = i2c_void;                     /* Обработчик завершения мастера */
fptr_t i2c_slavedone = i2c_slave_done_default;        /* Обработчик завершения слейва */
fptr_t i2c_softerror = i2c_void;                      /* Обработчик софтовой ошибки */
fptr_t i2c_harderror = i2c_void;                      /* Обработчик хардовой ошибки */

/* Флаги прерываний
 * NACKF - В режиме мастера мы отослали данные, а получили NACK или слэйв не ответил на свой адрес
 * TXIS - Регистр данных пуст, но есть данные для отправки (NBYTES != NBYTES_SENDED)
 * TC - передача завершена полностью (NBYTES = NBYTES_SENDED) и AUTOEND = 0
 * TCR - Передача завершена, но выставлен бит RELOAD, необходимо подготовить следующие данные для отправки
 * ADDR - В режиме слэйва совпал адрес (к нам обращаются), необходимо сбросить вручную, выставив ADDRCF
 * RXNE - Регистр данных приемника не пуст, необходимо вычитать данные
 */
void i2c_start(void)
{
  i2c_state |= I2C_BUS_BUSY; /**/

#ifdef I2C_SLAVE_EN							/*Если режим слейва разрешен - выключаем свой адрес и прерывание по совпадению*/
#ifndef I2C_MASTER_INT_EN
  I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
  I2C1->CR1 &= ~I2C_CR1_ADDRIE;
#endif
#endif

  I2C_SET_SLAVE_ADDR(i2c_slaveaddr); /*(1)*/
  switch (i2c_state & I2C_MODE_MSK)
  {
  case I2C_MODE_SWSW:
  case I2C_MODE_SWSR:
    I2C_SET_NBYTES_W(i2c_membuffer_nbytes); /*Кол-во байт адреса памяти и флаг записи*/
    i2c_state |= I2C_WRITE_MEM_ADDR; /*флаг того, что пишем адрес памяти*/
    I2C1->CR2 &= ~I2C_CR2_AUTOEND;
    break;

  case I2C_MODE_SW:
    I2C_SET_NBYTES_W(i2c_master_nbytes); /*Кол-во байт данных и флаг записи*/
    //I2C1->CR2 &= ~I2C_CR2_AUTOEND;
    I2C1->CR2 |= I2C_CR2_AUTOEND;
    break;

  case I2C_MODE_SR:
    I2C_SET_NBYTES_R(i2c_master_nbytes); /*Кол-во байт данных и флаг чтения*/
    //I2C1->CR2 &= ~I2C_CR2_AUTOEND;
    I2C1->CR2 |= I2C_CR2_AUTOEND;
    break;

  default:
    break;
  }
  i2c_state |= I2C_STARTED; /*Выставляем флаг старта*/
  i2c_state |= I2C_MASTER_WORKING; /*Майстар працуе*/
  I2C1->CR2 |= I2C_CR2_START; /*Go!*/

#ifdef I2C_SUPERVISOR_EN
  os_set_timer_task(i2c_supervisor, I2C_SUPERVISOR_DELAY);
#endif
}

static void i2c_buserror(uint32_t isrstatus)
{
  if (isrstatus & I2C_ISR_BERR)
    i2c_state |= I2C_ERR_BF; /*Ставим флаг*/

  if (isrstatus & I2C_ISR_ARLO)
    i2c_state |= I2C_ERR_EP; /*Ставим флаг*/

  if (i2c_state & I2C_MASTER_WORKING)
    i2c_state |= I2C_MASTER_INTERRUPTED;

  I2C1->ICR |= I2C_ICR_BERRCF; /*Сбрасываем флаги прерываний*/
  I2C1->ICR |= I2C_ICR_ARLOCF;
  I2C1->ICR |= I2C_ICR_STOPCF;
  i2c_harderror(); /*Прыгаем на обработчик*/
}

static void i2c_receiveaddr(uint32_t isrstatus)
{
  i2c_state |= I2C_SLAVE_WORKING | I2C_BUS_BUSY;
  /* USART_OutBufPUSH('A'); */
  if (i2c_state & I2C_MASTER_WORKING)
  {
    i2c_state |= I2C_MASTER_INTERRUPTED;
    i2c_state &= ~I2C_MASTER_WORKING;
  }
  if (isrstatus & I2C_ISR_DIR)
  {
    i2c_slave_outbuffer_index = 0;
    i2c_state &= ~I2C_SLAVE_RECEIVED;
    I2C1->ISR |= I2C_ISR_TXE;
  }
  else
  {
    i2c_state |= I2C_SLAVE_RECEIVED;
    i2c_slave_inbufindex = 0;
    if (I2C_SLAVE_IN_BUFSIZE == 1)
      I2C1->CR2 |= I2C_CR2_NACK;
  }
  I2C1->ICR |= I2C_ICR_STOPCF;
  I2C1->ICR |= I2C_ICR_ADDRCF; /*Сброс флага*/
  if (I2C1->ISR & I2C_ISR_RXNE)
  {
    i2c_slave_inbuffer[i2c_slave_inbufindex] = I2C1->RXDR; /*Забираем байт*/
    i2c_slave_inbufindex++;
  }
  else if (isrstatus & I2C_ISR_TXIS)
  {
    I2C1->TXDR = i2c_slave_outbuffer[i2c_slave_outbuffer_index];
    if (i2c_slave_outbuffer_index < I2C_SLAVE_OUT_BUFSIZE)
      i2c_slave_outbuffer_index++;
  }
}

static void i2c_receivenack(uint32_t isrstatus)
{
  if (i2c_state & I2C_SLAVE_WORKING) /*Мы в режиме слэйва*/
    I2C1->ICR |= I2C_ICR_NACKCF;
  else if (i2c_state & I2C_MASTER_WORKING)
  { /*Мы в режиме мастера*/
    if (i2c_state & I2C_STARTED) /*NACK Сразу после старта, слейв не отвечает*/
      i2c_state |= I2C_ERR_NA; /*Пишем код ошибки*/
    else
      /*Слейв отправил при передаче*/
      i2c_state |= I2C_ERR_NK;

    if ((isrstatus & I2C_ISR_STOPF) == 0) /*Если стопа на шине небыло -вкатываем*/
      I2C1->CR2 |= I2C_CR2_STOP;

    I2C1->ICR |= I2C_ICR_NACKCF; /*Сбрасываем флаг*/
  }
}

static void i2c_txcomplete(void)
{
  if (i2c_state & I2C_WRITE_MEM_ADDR) /*Записали адрес*/
  {
    i2c_state &= ~I2C_WRITE_MEM_ADDR; /*Снимаем флаг записи страницы*/
    if ((i2c_state & I2C_MODE_MSK) == I2C_MODE_SWSW)
      I2C_SET_NBYTES_W(i2c_master_nbytes); /*Режим записи*/
    else
      I2C_SET_NBYTES_R(i2c_master_nbytes); /*Режим чтения*/

    I2C1->CR2 |= I2C_CR2_START; /*Go!*/
    I2C1->CR2 |= I2C_CR2_AUTOEND;
  }
  else /*Отправили/приняли данные*/
  {
    I2C1->CR2 |= I2C_CR2_STOP; /*Все, стоп*/
  }
  //USART_OutBufPUSH('T');
}

static void i2c_txbyte(void)
{
  if (i2c_state & I2C_SLAVE_WORKING) /*Мы в режиме слэйва*/
  {
    I2C1->TXDR = i2c_slave_outbuffer[i2c_slave_outbuffer_index];
    if (i2c_slave_outbuffer_index < I2C_SLAVE_OUT_BUFSIZE)
    {
      i2c_slave_outbuffer_index++;
    }
    //USART_OutBufPUSH('d');
  }
  else if (i2c_state & I2C_MASTER_WORKING) /*Мы в режиме мастера*/
  {
    i2c_state &= ~I2C_STARTED; /*Сбросим флаг старта, мы уже в работе*/
    if (i2c_state & I2C_WRITE_MEM_ADDR) /*Пишем адрес?*/
    {
      I2C1->TXDR = i2c_membuffer[i2c_membuffer_index]; /*Скармливаем байт адреса*/
      i2c_membuffer_index++;
      //USART_OutBufPUSH('M');
    }
    else /*Просто данные*/
    {
      I2C1->TXDR = i2c_master_buffer[i2c_master_buffer_index]; /*Скармливаем байт данных*/
      i2c_master_buffer_index++; /*Увеличиваем индекс буфера*/
      //USART_OutBufPUSH('D');
    }
  }
}

static void i2c_rxbyte(void)
{
  if (i2c_state & I2C_SLAVE_WORKING) /*Мы в режиме слэйва*/
  {
    i2c_slave_inbuffer[i2c_slave_inbufindex] = I2C1->RXDR; /*Забираем байт*/
    i2c_slave_inbufindex++;
    //USART_OutBufPUSH('r');
    if (i2c_slave_inbufindex == I2C_SLAVE_IN_BUFSIZE - 1) /*Если место только под байт осталось, шлем Nack*/
    {
      I2C1->CR2 |= I2C_CR2_NACK;
      //USART_OutBufPUSH('n');
    }
  }
  else if (i2c_state & I2C_MASTER_WORKING) /*Мы в режиме мастера*/
  {
    i2c_state &= ~I2C_STARTED; /*Сбросим флаг старта, мы уже в работе*/
    i2c_master_buffer[i2c_master_buffer_index] = I2C1->RXDR; /*Положим байтик в буфер мастера*/
    i2c_master_buffer_index++; /*Увеличиваем индекс буфера*/
  }
}

static void i2c_stop(void)
{
  I2C1->ICR |= I2C_ICR_STOPCF; /*Сбрасываем флаг*/
  if (i2c_state & I2C_ERR_MSK) /*Если есть ошибки на шине - обрабатываем*/
  {
#ifdef I2C_SUPERVISOR_EN
    os_clear_timer_task(i2c_supervisor);
#endif
    i2c_softerror();
  }
  else
  {
    if (i2c_state & I2C_MASTER_WORKING) /*Иначе просто завершаем работу*/
    {
      i2c_state &= ~I2C_MASTER_WORKING;
#ifdef I2C_SUPERVISOR_EN
      os_clear_timer_task(i2c_supervisor);
#endif
#ifdef I2C_SLAVE_EN                    /*Если режим слейва разрешен - включаем свой адрес и прерывание по совпадению*/
      I2C1->OAR1 |= I2C_OAR1_OA1EN;
      I2C1->CR1 |= I2C_CR1_ADDRIE;
#endif
      i2c_masterdone();
    }
    else if (i2c_state & I2C_SLAVE_WORKING)
    {
      i2c_state &= ~I2C_SLAVE_WORKING;
      i2c_slavedone();
    }
  }
}

void i2c_slave_done_default(void)
{
  i2c_state = 0;
}

void i2c_master_reset_state(void)
{
  i2c_master_nbytes = 0;
  i2c_master_buffer_index = 0;
  i2c_membuffer_index = 0;
  i2c_membuffer_nbytes = 0;
  i2c_masterdone = i2c_void;
  i2c_softerror = i2c_void;
  i2c_state = 0;
}

void i2c_master_reset_index(void)
{
  i2c_master_buffer_index = 0;
  i2c_membuffer_index = 0;
}

uint8_t i2c_master_buffer_push(uint8_t data)
{
  if (i2c_master_nbytes == I2C_MASTER_BUFSIZE)
    return 1;

  i2c_master_buffer[i2c_master_buffer_index + i2c_master_nbytes] = data;
  i2c_master_nbytes++;

  return 0;
}

#ifdef I2C_SUPERVISOR_EN
void i2c_supervisor(void)
{
  if (I2C_MODULE_BUSY || I2C_BUS_ISBUSY)
  {
    i2c_module_reinit();
    i2c_master_reset_index();
    i2c_state |= I2C_ERR_SU;
    i2c_harderror();
  }
}
#endif

void i2c_void(void)
{

}

/* Алгоритм передачи в режиме мастер:
 *  Инициализация:
 *    (1) Пишем адрес слэйва
 *    (2) Задаем направление передачи - W
 *    (3) Если пишем сразу данные, записываем NBYTES данных, иначе NBYTES адреса памяти
 *    (4) AUTOEND-0
 *    (5) Врубаем прерывания TXIE, TCIE, NACKIE, ERRIE
 *    (6) Шлем старт
 *    (7) Выставляем в статус, что был старт
 *  Работа:
 *    (8) Если после старта вывалились в NACKF значит слейв не отвечает - пишем код ошибки NA и выход
 *    (9) Если NACKF после отправки байта - значит слэйв нас послал - пишем код NK
 *    (10) TXIS - скармливаем байт
 *    (11) TC - Отправили все данные. Если это был последний байт адреса, то -> (3)
 *    (12) Шлем стоп
 *    (13) Если режим слэйва разрешен - переключаемся в него
 */
/* Алгоритм работы слэйва
 *  () Ловим свой алрес
 *  () Сбрасываем флаг прерывания по совпадению адреса
 *  () Смотрим направление предачи если нам передавать - ставим флаг TXIS
 *    Передача:
 *      ()
 *
 * */

void I2C1_IRQHandler(void) /*Прерывание I2C*/
{
  uint32_t i2c_intstatus = I2C1->ISR;

  if (i2c_intstatus & (I2C_ISR_BERR | I2C_ISR_ARLO)) /*Ошибка на шине или потеря приоритета*/
  {
    i2c_buserror(i2c_intstatus);
    return;
  }

  if (i2c_intstatus & I2C_ISR_ADDR) /*Словили свой адрес*/
    i2c_receiveaddr(i2c_intstatus);

  if (i2c_intstatus & I2C_ISR_NACKF) /*Нас послали NACK*/
    i2c_receivenack(i2c_intstatus);

  if (i2c_intstatus & I2C_ISR_TC) /*Передача окончена*/
    i2c_txcomplete();

  if (i2c_intstatus & I2C_ISR_TXIS) /*Выходной буфер хочет жрат*/
    i2c_txbyte();

  if (i2c_intstatus & I2C_ISR_RXNE) /*В нас прилетел байт*/
    i2c_rxbyte();

  if (i2c_intstatus & I2C_ISR_STOPF) /* Отработали */
    i2c_stop();
}
#endif
