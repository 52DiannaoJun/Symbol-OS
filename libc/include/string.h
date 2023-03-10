#ifndef _STRING_H
#define _STRING_H

#include <stdarg.h>

#ifndef NULL
#define NULL ((void *)0)
#endif // NULL

void strlwr(char*str);
void strupr(char*str);
int strcmp(char*str0,char*str1);
int strcmpn(char*str0,char*str1,int num);
int isspace(char);
#endif // _STDIO_H
