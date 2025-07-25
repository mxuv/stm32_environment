#include <stdint.h>

#include "i2c.h"
#include "i2c_hal.h"

#ifdef I2C_EN
uint16_t i2c_state = 0;                  							/* ���������� ��������� IIC */
uint8_t i2c_slaveaddr;                                /* ����� ������ */

uint8_t i2c_membuffer[I2C_MEM_ADDRSIZE];              /* ����� ������ */
uint8_t i2c_membuffer_index = 0;
uint8_t i2c_membuffer_nbytes = 0;

uint8_t i2c_master_buffer_index = 0;
uint8_t i2c_master_nbytes = 0;
uint8_t i2c_master_buffer[I2C_MASTER_BUFSIZE];        /* ����� � ������ ������� */


uint8_t i2c_slave_inbufindex = 0;
uint8_t i2c_slave_inbuffer[I2C_SLAVE_IN_BUFSIZE];     /* ����� � ������ ������ �� ����� */
uint8_t i2c_slave_outbuffer_index = 0;
uint8_t i2c_slave_outbuffer[I2C_SLAVE_OUT_BUFSIZE];   /* ����� � ������ ������� �� �������� */

fptr_t i2c_masterdone = i2c_void;                     /* ���������� ���������� ������� */
fptr_t i2c_slavedone = i2c_slave_done_default;        /* ���������� ���������� ������ */
fptr_t i2c_softerror = i2c_void;                      /* ���������� �������� ������ */
fptr_t i2c_harderror = i2c_void;                      /* ���������� �������� ������ */

/* ����� ����������
 * NACKF - � ������ ������� �� �������� ������, � �������� NACK ��� ����� �� ������� �� ���� �����
 * TXIS - ������� ������ ����, �� ���� ������ ��� �������� (NBYTES != NBYTES_SENDED)
 * TC - �������� ��������� ��������� (NBYTES = NBYTES_SENDED) � AUTOEND = 0
 * TCR - �������� ���������, �� ��������� ��� RELOAD, ���������� ����������� ��������� ������ ��� ��������
 * ADDR - � ������ ������ ������ ����� (� ��� ����������), ���������� �������� �������, �������� ADDRCF
 * RXNE - ������� ������ ��������� �� ����, ���������� �������� ������
 */
void i2c_start(void)
{
  i2c_state |= I2C_BUS_BUSY; /**/

#ifdef I2C_SLAVE_EN							/*���� ����� ������ �������� - ��������� ���� ����� � ���������� �� ����������*/
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
    I2C_SET_NBYTES_W(i2c_membuffer_nbytes); /*���-�� ���� ������ ������ � ���� ������*/
    i2c_state |= I2C_WRITE_MEM_ADDR; /*���� ����, ��� ����� ����� ������*/
    I2C1->CR2 &= ~I2C_CR2_AUTOEND;
    break;

  case I2C_MODE_SW:
    I2C_SET_NBYTES_W(i2c_master_nbytes); /*���-�� ���� ������ � ���� ������*/
    //I2C1->CR2 &= ~I2C_CR2_AUTOEND;
    I2C1->CR2 |= I2C_CR2_AUTOEND;
    break;

  case I2C_MODE_SR:
    I2C_SET_NBYTES_R(i2c_master_nbytes); /*���-�� ���� ������ � ���� ������*/
    //I2C1->CR2 &= ~I2C_CR2_AUTOEND;
    I2C1->CR2 |= I2C_CR2_AUTOEND;
    break;

  default:
    break;
  }
  i2c_state |= I2C_STARTED; /*���������� ���� ������*/
  i2c_state |= I2C_MASTER_WORKING; /*������� ������*/
  I2C1->CR2 |= I2C_CR2_START; /*Go!*/

#ifdef I2C_SUPERVISOR_EN
  os_set_timer_task(i2c_supervisor, I2C_SUPERVISOR_DELAY);
#endif
}

static void i2c_buserror(uint32_t isrstatus)
{
  if (isrstatus & I2C_ISR_BERR)
    i2c_state |= I2C_ERR_BF; /*������ ����*/

  if (isrstatus & I2C_ISR_ARLO)
    i2c_state |= I2C_ERR_EP; /*������ ����*/

  if (i2c_state & I2C_MASTER_WORKING)
    i2c_state |= I2C_MASTER_INTERRUPTED;

  I2C1->ICR |= I2C_ICR_BERRCF; /*���������� ����� ����������*/
  I2C1->ICR |= I2C_ICR_ARLOCF;
  I2C1->ICR |= I2C_ICR_STOPCF;
  i2c_harderror(); /*������� �� ����������*/
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
  I2C1->ICR |= I2C_ICR_ADDRCF; /*����� �����*/
  if (I2C1->ISR & I2C_ISR_RXNE)
  {
    i2c_slave_inbuffer[i2c_slave_inbufindex] = I2C1->RXDR; /*�������� ����*/
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
  if (i2c_state & I2C_SLAVE_WORKING) /*�� � ������ ������*/
    I2C1->ICR |= I2C_ICR_NACKCF;
  else if (i2c_state & I2C_MASTER_WORKING)
  { /*�� � ������ �������*/
    if (i2c_state & I2C_STARTED) /*NACK ����� ����� ������, ����� �� ��������*/
      i2c_state |= I2C_ERR_NA; /*����� ��� ������*/
    else
      /*����� �������� ��� ��������*/
      i2c_state |= I2C_ERR_NK;

    if ((isrstatus & I2C_ISR_STOPF) == 0) /*���� ����� �� ���� ������ -���������*/
      I2C1->CR2 |= I2C_CR2_STOP;

    I2C1->ICR |= I2C_ICR_NACKCF; /*���������� ����*/
  }
}

static void i2c_txcomplete(void)
{
  if (i2c_state & I2C_WRITE_MEM_ADDR) /*�������� �����*/
  {
    i2c_state &= ~I2C_WRITE_MEM_ADDR; /*������� ���� ������ ��������*/
    if ((i2c_state & I2C_MODE_MSK) == I2C_MODE_SWSW)
      I2C_SET_NBYTES_W(i2c_master_nbytes); /*����� ������*/
    else
      I2C_SET_NBYTES_R(i2c_master_nbytes); /*����� ������*/

    I2C1->CR2 |= I2C_CR2_START; /*Go!*/
    I2C1->CR2 |= I2C_CR2_AUTOEND;
  }
  else /*���������/������� ������*/
  {
    I2C1->CR2 |= I2C_CR2_STOP; /*���, ����*/
  }
  //USART_OutBufPUSH('T');
}

static void i2c_txbyte(void)
{
  if (i2c_state & I2C_SLAVE_WORKING) /*�� � ������ ������*/
  {
    I2C1->TXDR = i2c_slave_outbuffer[i2c_slave_outbuffer_index];
    if (i2c_slave_outbuffer_index < I2C_SLAVE_OUT_BUFSIZE)
    {
      i2c_slave_outbuffer_index++;
    }
    //USART_OutBufPUSH('d');
  }
  else if (i2c_state & I2C_MASTER_WORKING) /*�� � ������ �������*/
  {
    i2c_state &= ~I2C_STARTED; /*������� ���� ������, �� ��� � ������*/
    if (i2c_state & I2C_WRITE_MEM_ADDR) /*����� �����?*/
    {
      I2C1->TXDR = i2c_membuffer[i2c_membuffer_index]; /*����������� ���� ������*/
      i2c_membuffer_index++;
      //USART_OutBufPUSH('M');
    }
    else /*������ ������*/
    {
      I2C1->TXDR = i2c_master_buffer[i2c_master_buffer_index]; /*����������� ���� ������*/
      i2c_master_buffer_index++; /*����������� ������ ������*/
      //USART_OutBufPUSH('D');
    }
  }
}

static void i2c_rxbyte(void)
{
  if (i2c_state & I2C_SLAVE_WORKING) /*�� � ������ ������*/
  {
    i2c_slave_inbuffer[i2c_slave_inbufindex] = I2C1->RXDR; /*�������� ����*/
    i2c_slave_inbufindex++;
    //USART_OutBufPUSH('r');
    if (i2c_slave_inbufindex == I2C_SLAVE_IN_BUFSIZE - 1) /*���� ����� ������ ��� ���� ��������, ���� Nack*/
    {
      I2C1->CR2 |= I2C_CR2_NACK;
      //USART_OutBufPUSH('n');
    }
  }
  else if (i2c_state & I2C_MASTER_WORKING) /*�� � ������ �������*/
  {
    i2c_state &= ~I2C_STARTED; /*������� ���� ������, �� ��� � ������*/
    i2c_master_buffer[i2c_master_buffer_index] = I2C1->RXDR; /*������� ������ � ����� �������*/
    i2c_master_buffer_index++; /*����������� ������ ������*/
  }
}

static void i2c_stop(void)
{
  I2C1->ICR |= I2C_ICR_STOPCF; /*���������� ����*/
  if (i2c_state & I2C_ERR_MSK) /*���� ���� ������ �� ���� - ������������*/
  {
#ifdef I2C_SUPERVISOR_EN
    os_clear_timer_task(i2c_supervisor);
#endif
    i2c_softerror();
  }
  else
  {
    if (i2c_state & I2C_MASTER_WORKING) /*����� ������ ��������� ������*/
    {
      i2c_state &= ~I2C_MASTER_WORKING;
#ifdef I2C_SUPERVISOR_EN
      os_clear_timer_task(i2c_supervisor);
#endif
#ifdef I2C_SLAVE_EN                    /*���� ����� ������ �������� - �������� ���� ����� � ���������� �� ����������*/
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

/* �������� �������� � ������ ������:
 *  �������������:
 *    (1) ����� ����� ������
 *    (2) ������ ����������� �������� - W
 *    (3) ���� ����� ����� ������, ���������� NBYTES ������, ����� NBYTES ������ ������
 *    (4) AUTOEND-0
 *    (5) ������� ���������� TXIE, TCIE, NACKIE, ERRIE
 *    (6) ���� �����
 *    (7) ���������� � ������, ��� ��� �����
 *  ������:
 *    (8) ���� ����� ������ ���������� � NACKF ������ ����� �� �������� - ����� ��� ������ NA � �����
 *    (9) ���� NACKF ����� �������� ����� - ������ ����� ��� ������ - ����� ��� NK
 *    (10) TXIS - ����������� ����
 *    (11) TC - ��������� ��� ������. ���� ��� ��� ��������� ���� ������, �� -> (3)
 *    (12) ���� ����
 *    (13) ���� ����� ������ �������� - ������������� � ����
 */
/* �������� ������ ������
 *  () ����� ���� �����
 *  () ���������� ���� ���������� �� ���������� ������
 *  () ������� ����������� ������� ���� ��� ���������� - ������ ���� TXIS
 *    ��������:
 *      ()
 *
 * */

void I2C1_IRQHandler(void) /*���������� I2C*/
{
  uint32_t i2c_intstatus = I2C1->ISR;

  if (i2c_intstatus & (I2C_ISR_BERR | I2C_ISR_ARLO)) /*������ �� ���� ��� ������ ����������*/
  {
    i2c_buserror(i2c_intstatus);
    return;
  }

  if (i2c_intstatus & I2C_ISR_ADDR) /*������� ���� �����*/
    i2c_receiveaddr(i2c_intstatus);

  if (i2c_intstatus & I2C_ISR_NACKF) /*��� ������� NACK*/
    i2c_receivenack(i2c_intstatus);

  if (i2c_intstatus & I2C_ISR_TC) /*�������� ��������*/
    i2c_txcomplete();

  if (i2c_intstatus & I2C_ISR_TXIS) /*�������� ����� ����� ����*/
    i2c_txbyte();

  if (i2c_intstatus & I2C_ISR_RXNE) /*� ��� �������� ����*/
    i2c_rxbyte();

  if (i2c_intstatus & I2C_ISR_STOPF) /* ���������� */
    i2c_stop();
}
#endif
