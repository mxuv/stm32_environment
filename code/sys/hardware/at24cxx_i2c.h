#ifndef __AT24CXX_I2C_H__
#define __AT24CXX_I2C_H__

#include "nstdtypes.h"
#include "hal_config.h"

/**#define AT24CXX_ADDR			0b10100000**/
/* #define AT24CXX_ADDR			0b10100010 */
/* #define AT24CXX_ADDR			0b10100100 */
/* #define AT24CXX_ADDR			0b10100110 */
/* #define AT24CXX_ADDR			0b10101000 */
/* #define AT24CXX_ADDR			0b10101010 */
/* #define AT24CXX_ADDR			0b10101100 */
/* #define AT24CXX_ADDR			0b10101110 */
#ifndef AT24CXX_ADDR
#define AT24CXX_ADDR			0b10101110
#endif

uint8_t at24_write(uint8_t chipaddr, uint16_t memaddr, uint8_t n_bytes,
    fptr_t callback_done, fptr_t callback_err);

uint8_t at24_read(uint8_t chipaddr, uint16_t memaddr, uint8_t n_bytes,
    fptr_t callback_done, fptr_t callback_err);

#endif
