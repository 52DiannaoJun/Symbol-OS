#ifndef _SYM_OS_INT_H_
#define _SYM_OS_INT_H_
#include "./func.h"

#define PORT_KEYDAT   0x0060
#define PORT_KEYSTA   0x0064
#define PORT_KEYCMD   0x0064

#define PIC0_ICW1   0x0020
#define PIC0_ICW2   0x0021
#define PIC0_ICW3   0x0021
#define PIC0_ICW4   0x0021
#define PIC0_OCW2   0x0020
#define PIC0_IMR    0x0021

#define PIC1_ICW1   0x00a0
#define PIC1_ICW2   0x00a1
#define PIC1_ICW3   0x00a1
#define PIC1_ICW4   0x00a1
#define PIC1_OCW2   0x00a0
#define PIC1_IMR    0x00a1
#define PIC_M_CTRL 0x20
#define PIC_M_DATA 0x21
#define PIC_S_CTRL 0xa0
#define PIC_S_DATA 0xa1

#define PIT_CNT0    0x0040
#define PIT_CTRL    0x0043

#include "./io/keybard.h"
#include "./io/floppy.h"
#include "./io/hardisk.h"
#include "./fs.h"
#include "./time.h"

void init_pic(void);
void init_pit(void);
void int20_c(int*);
void int21_c(int*);
void int27_c(int*);
void int2c_c(int*);
void int40_c(int,int,int,int,int,int,int,int);
void int13_c(int,int,int,int,int,int,int,int);
void int0d_c(int,int,int,int,int,int,int,int);
void int20_asm(void);
void int21_asm(void);
void int27_asm(void);
void int2c_asm(void);
void int40_asm(void);
void int13_asm(void);
void int0d_asm(void);

void int20_c(int *esp){			//timer
  out8(PIC0_OCW2, 0x60);
  timer.cnt++;
  return;
}

void int21_c(int *esp){			//keyboard
  out8(PIC0_OCW2,0x61);
  setKey(in8(PORT_KEYDAT));
  return;
}

void int27_c(int *esp){			//none
  out8(PIC0_OCW2, 0x67);
  return;
}

void int2c_c(int *esp){			//mouse
  out8(PIC1_OCW2, 0x64);
	out8(PIC0_OCW2, 0x6c);
  setMos(in8(PORT_KEYDAT));
	return;
}

void int40_c(int edi,int esi,int ebp,int esp,int ebx,int edx,int ecx,int eax){
  //char buf[32];
  char*prt=0;
  out8(PIC0_OCW2, 0x80);
  switch(eax){
	case 0x10:
	  if(hasKey())
	    prt[ebx]=getC(),keybuf.flag=1;
	  else
		prt[ebx]=0;
	  break;
	case 0x11:
	  if(hasKey())
	    prt[ebx]=getKey(),keybuf.flag=1;
	  else
		prt[ebx]=0;
	  break;
  }
  return;
}

void int13_c(int edi,int esi,int ebp,int esp,int ebx,int edx,int ecx,int eax){
  return;
}
void int0d_c(int edi,int esi,int ebp,int esp,int ebx,int edx,int ecx,int eax){
  if(eax==0)
    putstr((ScrnInfo*)0x0ff0,0,16,RGB_FFFFFF,"System Error!!! "),
    putstr((ScrnInfo*)0x0ff0,1,15,RGB_FF0000,"System Error!!! ");
  else
    putstr((ScrnInfo*)0x0ff0,0,16,RGB_FFFFFF,(char*)eax),
    putstr((ScrnInfo*)0x0ff0,1,15,RGB_FF0000,(char*)eax);
  for(;; asm_hlt());
  return;
}

void init_pic(void){
  out8(PIC0_IMR,0xff);
  out8(PIC1_IMR,0xff);
	
  out8(PIC0_ICW1,0x11);
  out8(PIC0_ICW2,0x20);
  out8(PIC0_ICW3,1<<2);
  out8(PIC0_ICW4,0x01);
	
  out8(PIC1_ICW1,0x11);
  out8(PIC1_ICW2,0x28);
  out8(PIC1_ICW3,0x02);
  out8(PIC1_ICW4,0x01);
	
  out8(PIC0_IMR,0xfb);
  out8(PIC1_IMR,0xff);
  return;
}
void init_pit(void){
  out8(PIT_CTRL,0x34);
  out8(PIT_CNT0,0x9c);
  out8(PIT_CNT0,0x2e);
  return;
}

#endif