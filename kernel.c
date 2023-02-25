#include <stdio.h>
#include "library/func.h"
#include "library/graph.h"
#include "library/int.h"
#include "library/segment.h"

u8*img=(u8*)0x160000;
#include "library/cmd.c"

void init_othr(void);

int main(void) {
  ScrnInfo *scnini = (ScrnInfo *)0x0ff0;

  init_gdtidt();
  init_pic();
  init_pit();
  asm_sti();
  out8(PIC0_IMR, 0xf8);
  out8(PIC1_IMR, 0xef);
  init_Floppy();
  init_palette();
  init_screen(scnini);
  init_othr();
  for (;;) {
    asm_hlt();
  put_fonts8_asc(scnini,0, 64, 8,"0");
	  if(hasC()&&getC()==' ')
      put_fonts8_asc(scnini,0, 64, 8,"1"),
      CMD_main();
  }
  return 0;
}

void init_othr(void){
  SysInfo.memlen=sys_memcheck(0x100000,0xffffffff)/1024+1024;
  SysInfo.lang=Lang_EN;
  Msgs[Lang_EN][Word_WELCOM]=Word_WELCOM_EN;
  Msgs[Lang_EN][Word_WRNCMD]=Word_WRNCMD_EN;
  Msgs[Lang_CN_SP][Word_WELCOM]=Word_WELCOM_ZH_SP;
  Msgs[Lang_CN_SP][Word_WRNCMD]=Word_WRNCMD_ZH_SP;
  return;
}