#include <stdint.h>

void stringcopy(char* dest, const char* src)
{
  while(*src)
  {
    *dest = *src;
    dest++;
    src++;
  }
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
