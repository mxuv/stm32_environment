#ifndef _STR_H_
#define _STR_H_

char* sstringcopynz(char *dest, const char *src, int cnt);
char* stringcopynz(char *dest, const char *src);
void stringcopy(char *dest, const char *src);
void sstringcopy(char *dest, const char *src, int cnt);
char* skipchar(const char *str, char ch);
int stringlen(const char *str);

void stripzeros(char *str);
void str2fxp(char *str, int pindex, int bp, int ap);
void sint_strz(int size, int32_t number, char *str);
void uint_strz(int size, uint32_t number, char *str);
void uint_str(int size, uint32_t number, char *str);
void sint_str(int size, uint32_t number, char *str);


#endif
