#include <stdint.h>
#include <stddef.h>

#include "str.h"

int stringlen (const char *str)
{
  int len = 0;
  while (*str) {
    len++;
    str++;
  }
  return len;
}

void stringcopy(char* dest, const char* src)
{
  while(*src)
  {
    *dest = *src;
    dest++;
    src++;
  }
}

const char* skipchar(const char *str, char ch)
{
  while (*str) {
    if (*str != ch)
      return str;
    str++;
  }
  return NULL;
}

const char* skipzeros(const char *str)
{
  const char *s;

  s = skipchar(str, '0');
  if (s == NULL)
    return str + stringlen(str) - 1;
  else
    return s;
}

void sstrcopy(char* dest, const char* src, uint8_t cnt)
{
  while(cnt)
  {
    *dest = *src;
    dest++;
    src++;
    cnt--;
  }
}
