#ifndef __NCONV_H__
#define __NCONV_H__

#include <stdint.h>


uint8_t mrank(uint32_t *number, uint32_t rank);
#if 1
void uint_str(uint8_t size, uint32_t number, char *str);
#else
void uint_str(uint8_t digits, uint32_t number, uint8_t *str);
#endif
/*void uint32_str(uint32_t number, uint8_t *str);*/

#endif
