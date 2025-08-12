#include <stdint.h>
#include <stddef.h>

#include "str.h"

char* sstringcopynz(char *dest, const char *src, int cnt)
{
    while (cnt) {
        *dest = *src;
        dest++;
        src++;
        cnt--;
    }
    return dest;
}

char* stringcopynz(char *dest, const char *src)
{
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    return dest;
}

void stringcopy(char *dest, const char *src)
{
    dest = stringcopynz(dest, src);
    *dest = '\0';
}

void sstringcopy(char *dest, const char *src, int cnt)
{
    dest = sstringcopynz(dest, src, cnt);
    *dest = '\0';
}

char* skipchar(const char *str, char ch)
{
    while (*str) {
        if (*str != ch)
            return (char*) str;
        str++;
    }
    return NULL;
}

void str2fxp(char *str, int pindex, int bp, int ap)
{
    char buf[15];
    char *p = buf;

    if (*str == '-') {
        *p = *str;
        p++;
    }

    p = sstringcopynz(p, str + (pindex - (bp - 1)), bp);
    *p++ = '.';
    p = sstringcopynz(p, str + (pindex + 1), ap);
    *p = '\0';
    stringcopy(str, buf);
}

int stringlen(const char *str)
{
    int len = 0;
    while (*str) {
        len++;
        str++;
    }
    return len;
}

void stripzeros(char *str)
{
    char *s;
    if (*str == '-')
        str++;

    s = skipchar(str, '0');
    if (s == NULL) {
        *str++ = '0';
        *str = '\0';
        return;
    } else {
        if (*s == '.')
            stringcopy(str, s - 1);
        else
            stringcopy(str, s);
    }
}

static uint8_t getrank(uint8_t size)
{
    switch (size) {
    case 1:
        return 3;
        break;
    case 2:
        return 5;
        break;
    case 4:
    default:
        return 10;
        break;
    }
}

void sint_strz(int size, int32_t number, char *str)
{
    uint32_t unum;
    if (number < 0) {
        unum = (uint32_t) (-(number + 1) + 1);
        *str = '-';
        str++;
    } else {
        unum = (uint32_t) number;
    }
    uint_strz(size, unum, str);
}

void uint_strz(int size, uint32_t number, char *str)
{
    uint8_t rank;

    rank = getrank(size);
    *(str + rank) = '\0';
    str += rank - 1;
    while (rank) {
        *str = (char) (number % 10) + '0';
        number /= 10;
        str--;
        rank--;
    }
}

void uint_str(int size, uint32_t number, char *str)
{
    uint_strz(size, number, str);
    stripzeros(str);
}

void sint_str(int size, uint32_t number, char *str)
{
    sint_strz(size, number, str);
    stripzeros(str);
}
