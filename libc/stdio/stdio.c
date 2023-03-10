#include <stdarg.h>
#include <stdio.h>

struct SprintBuf {
  char *buf;
  int count;
};

void printnum(void (*fputch)(char, void *), void *data, unsigned long num,
              int base) {
  if (num >= base) {
    printnum(fputch, data, num / base, base);
  }

  fputch("0123456789abcdef"[num % base], data);
}

void printnum_(void (*fputch)(char, void *), void *data, unsigned long num,
              int base) {
  if (num >= base) {
    printnum_(fputch, data, num / base, base);
  }

  fputch("0123456789ABCDEF"[num % base], data);
}

long inputnum(char (*fgetch)(void*),void *data,int base){
  static unsigned char val['g'-'0']={0,1,2,3,4,5,6,7,8,9};
  long ret=0;
  char ch;
  if(val['a'-'0']!=10)
    for(ch='a';ch<'g';ch++)
        val[ch-'0'-32]=val[ch-'0']=ch-'a'+10;
  while(((ch=fgetch(data))>'9'||ch<'0')&&(ch>'f'||ch<'a')&&(ch>'F'||ch<'A'));
  while((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')||(ch>='A'&&ch<='F')){
    ret*=base,ret+=val[ch-'0'],ch=fgetch(data);
  }
  return ret;
}

void vprintfmt(void (*fputch)(char, void *), void *data, const char *fmt,
               va_list ap) {
  int ch;
  unsigned long long num;
  char *str;
  bool plusSym=0,unSym=0;

  while (1) {
    while ((ch = *fmt++) != '%') {
      fputch(ch, data);
      if (ch == '\0') {
        return;
      }
    }

    num = 0;
    switch (ch = *fmt++) {
    case 'c':
      fputch(va_arg(ap, int), data);
      plusSym=0,unSym=0;
      break;

    case 'd':
      num = va_arg(ap, int);
      if ((long long)num<0 && !unSym) {
        fputch('-', data);
        num = -(long long)num;
      }
      else if(plusSym && ((unSym)||((long long)num>0))){
        fputch('+', data);
      }
      printnum(fputch, data, num, 10);
      plusSym=0,unSym=0;
      break;

    case 'p':
      fputch('0', data);
      fputch('x', data);
      num = (unsigned long)va_arg(ap, void *);
      printnum(fputch, data, num, 16);
      plusSym=0,unSym=0;
      break;

    case 'P':
      fputch('0', data);
      fputch('x', data);
      num = (unsigned long)va_arg(ap, void *);
      printnum_(fputch, data, num, 16);
      plusSym=0,unSym=0;
      break;

    case 'x':
      num = (unsigned long)va_arg(ap, void *);
      printnum(fputch, data, num, 16);
      plusSym=0,unSym=0;
      break;

    case 'X':
      num = (unsigned long)va_arg(ap, void *);
      printnum_(fputch, data, num, 16);
      plusSym=0,unSym=0;
      break;

    case 's':
      str = va_arg(ap, char *);
      if (str == NULL) {
        str = "<null>";
      }
      while (*str != '\0') {
        fputch(*str, data);
        str++;
      }
      plusSym=0,unSym=0;
      break;

    case '%':
      fputch('%', data);
      plusSym=0,unSym=0;
      break;

    case '+':
      plusSym=1;
      break;

    case 'u':
      unSym=1;
      break;

    default:
      plusSym=0;
      break;
    }
  }
}

void sprint_putch(char c, struct SprintBuf *data) {
  *data->buf++ = c;
  data->count++;
}
char sscan_getch(struct SprintBuf *data){
  return *data->buf++;
}
void vscanfmt(void (*fputch)(char, void *), void *data, const char *fmt,
               va_list ap) {return;}
int vsprintf(char *s, const char *format, va_list ap) {
  struct SprintBuf buf = {s, 0};

  vprintfmt((void *)sprint_putch, &buf, format, ap);

  return buf.count;
}
int vsscanf(char *s, const char *format, va_list ap) {
  struct SprintBuf buf = {s, 0};

  vscanfmt((void *)sprint_putch, &buf, format, ap);

  return buf.count;
}

int sprintf(char *s, const char *format, ...) {
  va_list ap;
  int ret;

  va_start(ap, format);
  ret = vsprintf(s, format, ap);
  va_end(ap);

  return ret;
}
int sscanf(char *s, const char *format, ...) {
  va_list ap;
  int ret;

  va_start(ap, format);
  ret = vsscanf(s, format, ap);
  va_end(ap);

  return ret;
}