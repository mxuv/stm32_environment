#ifndef __I2C_HAL_H__
#define __I2C_HAL_H__

#include "mcu.h"
#include "i2c_config.h"

#define I2C_PRESC_POS						  (I2C_TIMINGR_PRESC_Pos		)
#define I2C_SCLDEL_POS					  (I2C_TIMINGR_SCLDEL_Pos 	)
#define I2C_SDADEL_POS					  (I2C_TIMINGR_SDADEL_Pos		)
#define I2C_SCLH_POS						  (I2C_TIMINGR_SCLH_Pos			)
#define I2C_SCLL_POS						  I2C_TIMINGR_SCLL_Pos

#define I2C_PRESC_VAL						  (I2C_PRESC << I2C_PRESC_POS)
#define I2C_SCLDEL_VAL					  (I2C_SCLDEL << I2C_SCLDEL_POS)
#define I2C_SDADEL_VAL					  (I2C_SDADEL << I2C_SDADEL_POS)
#define I2C_SCLH_VAL						  (I2C_SCLH << I2C_SCLH_POS)
#define I2C_SCLL_VAL						  (I2C_SCLL << I2C_SCLL_POS)

//#define I2C_TIMINGR_VAL					0x00300641
#define I2C_TIMINGR_VAL					  (I2C_SCLL_VAL	| I2C_SCLH_VAL | I2C_SDADEL_VAL \
                                  | I2C_SCLDEL_VAL | I2C_PRESC_VAL)

#define I2C_ISR_MASK						  (I2C_ISR_ADDR | I2C_ISR_ARLO | I2C_ISR_BERR \
                                  | I2C_ISR_RXNE | I2C_ISR_TC | I2C_ISR_TCR \
                                  | I2C_ISR_TXIS | I2C_ISR_STOPF)

#define I2C_SET_NBYTES_R(nbytes)  (I2C1->CR2 = (uint32_t)(I2C1->CR2 &~(0xFF << 16)) \
                                  | (nbytes << 16) | I2C_CR2_RD_WRN)
#define I2C_SET_NBYTES_W(nbytes)  (I2C1->CR2 = (uint32_t)((I2C1->CR2 &~(0xFF << 16)) \
                                  | (nbytes << 16)) & (~I2C_CR2_RD_WRN))
#define I2C_SET_SLAVE_ADDR(addr)  (I2C1->CR2	= (uint32_t)(I2C1->CR2 &~(0x7F << 1)) \
                                  | addr)
#define I2C_MODULE_BUSY           (I2C1->ISR & I2C_ISR_BUSY)

void i2c_init(void);
void i2c_module_reinit(void);

#endif
