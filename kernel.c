#include <stdio.h>
#include "library/func.h"
#include "library/graph.h"
#include "library/int.h"
#include "library/segment.h"

u8*img=(u8*)0x160000;
#include "library/cmd.c"

int main(void) {
  ScrnInfo *scnini = (ScrnInfo *)0x0ff0;

  init_gdtidt();
  init_pic();
  init_Floppy();
//  init_KB();
  init_pit();
  asm_sti();
  out8(PIC0_IMR, 0xf8);
  out8(PIC1_IMR, 0xef);
  init_palette();
  init_screen(scnini);
  FUNCS[0]=(prtFunc*)CMD_main;

  for (;;) {
    asm_hlt();
	  //if(hasC()&&getC()==' ')
      CMD_main();
  }
  return 0;
}
