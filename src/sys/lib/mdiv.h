#ifndef _MDIV_H_
#define _MDIV_H_

#include <stdint.h>

#define MDIV_M10    0xCCCCCCCD
#define MDIV_S10    35
#define MDIV_M100   0x028F5C29
#define MDIV_S100   32

void udiv(uint32_t num, uint32_t div, uint32_t m,
    uint8_t s, uint32_t *q, uint32_t *r);

#define UDIV_10(n, q, r) udiv((n), 10, MDIV_M10, MDIV_S10, (q), (r))
#endif
