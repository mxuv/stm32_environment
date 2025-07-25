#include <stdint.h>

#include "at24cxx_i2c.h"
#include "i2c.h"

#ifdef AT24CXXI2C_EN
uint8_t at24_write(uint8_t chipaddr, uint16_t memaddr, uint8_t n_bytes,
    fptr_t callback_done, fptr_t callback_err)
{
  if (I2C_BUS_ISBUSY)
  {
    return 1;
  }

  i2c_slaveaddr = chipaddr;
  i2c_state = I2C_MODE_SW | I2C_BUS_BUSY;

  i2c_master_buffer[0] = HIGH(memaddr);
  i2c_master_buffer[1] = LOW(memaddr);

  i2c_master_buffer_index = 0;
  i2c_master_nbytes = n_bytes + 2;
  i2c_masterdone = callback_done;
  i2c_softerror = callback_err;

  i2c_start();

  return 0;
}

uint8_t at24_read(uint8_t chipaddr, uint16_t memaddr, uint8_t n_bytes,
    fptr_t callback_done, fptr_t callback_err)
{
  if (I2C_BUS_ISBUSY)
  {
    return 1;
  }

  i2c_slaveaddr = chipaddr;
  i2c_state = I2C_MODE_SWSR | I2C_BUS_BUSY;

  i2c_membuffer[0] = HIGH(memaddr);
  i2c_membuffer[1] = LOW(memaddr);
  i2c_membuffer_nbytes = 2;
  i2c_membuffer_index = 0;

  i2c_master_buffer_index = 0;
  i2c_master_nbytes = n_bytes;
  i2c_masterdone = callback_done;
  i2c_softerror = callback_err;

  i2c_start();

  return 0;
}

#endif
