#ifndef _SYM_OS_KEYBARD_H_
#define _SYM_OS_KEYBARD_H_

#include "../func.h"

#define k_Del		0x7f
#define k_Ins		0x80
#define k_Home	0x81
#define k_End		0x82
#define k_Pu		0x83
#define k_Pd		0x84
#define k_Up		0x85
#define k_Dn		0x86
#define k_Lf		0x87
#define k_Rt		0x88
#define k_LC		0x89
#define k_RC		0x8a
#define k_LS		0x8b
#define k_RS		0x8c
#define k_LA		0x8d
#define k_RA		0x8e
#define k_CL		0x8f
#define k_SL		0x90
#define k_NL		0x91
#define k_SI		0x92
#define k_St		0x93
#define k_Sd		0x94
#define k_Ss		0x95
#define k_Sp		0x96
#define k_Se		0x97
#define k_Sf		0x98
#define k_Fc		0xa0
#define KB_STA_BUSY 0x02
#define KB_WRT_MODE 0x60
#define KB_MODE_KBC 0x47
#define MS_STA_BUSY 0xf4
#define KB_MODE_MOS 0xd4

static char charset0[0x80]={
	0x00,'\e','1','2','3','4','5','6','7','8','9','0','-','=','\b',
	'\t','q','w','e','r','t','y','u','i','o','p','[',']','\r',k_LC,
	'a','s','d','f','g','h','j','k','l',';','\'','`',k_LS,'\\','z',
	'x','c','v','b','n','m',',','.','/',k_RS,k_St,k_LA,' ',k_CL,
	k_Fc+1,k_Fc+2,k_Fc+3,k_Fc+4,k_Fc+5,k_Fc+6,k_Fc+7,k_Fc+8,k_Fc+9,
	k_Fc+10,k_NL,k_SL,k_Home,k_Up,k_Pu,k_Ss,k_Lf,k_Sf,k_Rt,k_Sp,
	k_End,k_Dn,k_Pd,k_Ins,k_Del,k_SI,0x00,0x00,k_Fc+11,k_Fc+12,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,'_',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	'\\',0x00,0x00
},charset1[0x80]={
	0x00,'\e','!','@','#','$','%','^','&','*','(',')','_','+',k_Del,
	'\v','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',k_LC,
	'A','S','D','F','G','H','J','K','L',':','\"','~',k_LS,'|','Z',
	'X','C','V','B','N','M','<','>','?',k_RS,k_St,k_LA,' ',k_CL,
	k_Fc+1,k_Fc+2,k_Fc+3,k_Fc+4,k_Fc+5,k_Fc+6,k_Fc+7,k_Fc+8,k_Fc+9,
	k_Fc+10,k_NL,k_SL,'7','8','9','-','4','5','6','+',
	'1','2','3','0','1',k_SI,0x00,0x00,k_Fc+11,k_Fc+12,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,'_',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	'\\',0x00,0x00
};
struct _keyBuf_{
	short data;
	int flag:1,Kflag:1,Shift:1,Alt:1,Ctrl:1,Num:1,Caps:1;
};
struct _mosBuf_{
  short x,y;
  int flag:1,Kflag:1,Left:1,Right:1,Mid:1,Up:1,Down:1;
};
extern struct _keyBuf_ keybuf;
extern struct _mosBuf_ mousebuf;

static void setKey(short);
static char hasC(void);
static char hasKey(void);
static short getKey(void);
static char getC(void);

static char hasC(void){
  return keybuf.flag;
}

static char hasKey(void){
  return keybuf.Kflag|keybuf.flag;
}

static short getKey(void){
  if(hasKey()==0)
    return 0;
  keybuf.Kflag=0;
  return keybuf.data;
}

static char getC(void){
  char ret=0;
  if(hasC()==0) return 0;
  keybuf.flag=0;
  keybuf.Kflag=0;
  if(keybuf.Shift&&keybuf.Caps)ret=charset0[keybuf.data];
  else if(keybuf.Shift||keybuf.Caps)ret=charset1[keybuf.data];
  else ret=charset0[keybuf.data];
  if(keybuf.Ctrl)ret-='@';
  return ret;
}

static void setKey(short k){
  if(k<0x80){
    if(k==0x2a||k==0x36)keybuf.Shift=1;
    else if(k==0x1d)keybuf.Ctrl=1;
    else if(k==0x38)keybuf.Alt=1;
    else if(k==0x3a)keybuf.Caps=!keybuf.Caps;
    else keybuf.data=k,keybuf.flag=1;
    keybuf.Kflag=1;
  }else if(k<0x100){
    k&=0x7f;
    if(k==0x2a||k==0x36)keybuf.Shift=0;
    else if(k==0x1d)keybuf.Ctrl=0;
    else if(k==0x38)keybuf.Alt=0;
    //else if(k==0x3a)keybuf.Caps=!keybuf.Caps;
    else keybuf.data=k,keybuf.flag=0;
    keybuf.Kflag=0;
  }else keybuf.Kflag=1;
  return;
}

static void setMos(u8 k){
  static short i=2;
  static u8 buf[3];
  buf[i++]=k;
  if(k==3){
    k==0;
    mousebuf.flag=1;
  }
  return;
}

static void wait_KB_ready (void){
  while(in8(PORT_KEYSTA)&KB_STA_BUSY)
    asm_nop();
  return ;
}
static void init_KB (void){
  wait_KB_ready();
  out8(PORT_KEYCMD,KB_WRT_MODE);
  wait_KB_ready();
  out8(PORT_KEYDAT,KB_MODE_KBC);
  wait_KB_ready();
  out8(PORT_KEYCMD,KB_MODE_MOS);
  wait_KB_ready();
  out8(PORT_KEYDAT,MS_STA_BUSY);
  wait_KB_ready();
  return;
}
#endif