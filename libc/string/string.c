#include <stdarg.h>
#include <string.h>

void strlwr(char*str){
  while(*str){
  	if(*str>='A'&&*str<='Z')
  	  *str+=32;
  	str++;
  }
  return;
}
void strupr(char*str){
  while(*str){
  	if(*str>='a'&&*str<='z')
  	  *str-=32;
  	str++;
  }
  return;
}
int strcmp(char*str0,char*str1){

  while((*str0==*str1)&&(*str0)&&(*str1))
    str0++,str1++;
  return (*str0==*str1);
}
int strcmpn(char*str0,char*str1,int num){
  int prt=0;
  while((str0[prt]==str1[prt])&&(str0[prt])&&(str1[prt])&&(prt<num))
    prt++;
  return (str0[prt]==str1[prt]);
}
void strcat(char*to,char*from){
  while(*to)to++;
  while(*to=*from)to++,from++;
  return;
}

int isspace(char ch){
  if(ch-' ') if(ch-'\n')if(ch-'\r')if(ch-'\t')if(ch-'\v')if(ch)
    return 0;
  return 1;
}