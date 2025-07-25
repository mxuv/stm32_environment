#ifndef __NSTDTYPES_H__
#define __NSTDTYPES_H__

#include <stdint.h>

#define FOURTH(x)	((x) >> 24)
#define THIRD(x)	(((x) >> 16) & 0xFF)
#define HIGH(x) 	(((x) >> 8) & 0xFF)
#define LOW(x) 		((x) & 0xFF)

typedef void (*fptr_t)(void);

typedef union
{
  uint64_t unum;
  int64_t snum;
  uint8_t byte[8];
} num64_t;

typedef union
{
  uint32_t unum;
  int32_t snum;
  uint8_t byte[4];
} num32_t;

typedef union
{
  uint16_t unum;
  int16_t snum;
  uint8_t byte[2];
} num16_t;

#endif
