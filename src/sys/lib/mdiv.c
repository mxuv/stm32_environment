#include "mdiv.h"

void udiv(uint32_t num, uint32_t div, uint32_t m,
    uint8_t s, uint32_t *q, uint32_t *r)
{
  uint32_t temp;
  temp = (uint32_t)(((uint64_t)num * m) >> s);
  *q = temp;
  *r = num - div * temp;
}
