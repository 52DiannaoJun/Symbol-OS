#include "./lib/graph.h"
void main () {
    static char FLAGS0[3]={"SC0"};
	BootInfo *binfo = (BootInfo *)0x0ff0;
	put_fonts8_asc(binfo, 0, 0, 0, "TEST");
	return ;
}
// ind=(sec-31)*512+0x3e00
