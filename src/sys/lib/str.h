#ifndef __STR_H__
#define __STR_H__

void stringcopy(char* dest, const char* src);
const char* skipchar(const char *str, char ch);
const char* skipzeros(const char *str);
void sstrcopy(char* dest, const char* src, uint8_t count);

#endif
