#ifndef _SYM_OS_FUNC_H_
#define _SYM_OS_FUNC_H_
#define EFLAGS_AC_BIT 0x00040000
#define CR0_CACHE_DISABLE 0x60000000

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef unsigned char bool;
typedef unsigned char code;
typedef void prtFunc(void);

void asm_nop(void);
void asm_hlt(void);
void asm_cli(void);
void asm_sti(void);
void asm_stihlt(void);
void far_jmp(u32 eip, u32 cs);

void out8(u32 port, u32 data);
void out16(u32 port, u32 data);
void out32(u32 port, u32 data);
u32 in8(u32 port);
u32 in16(u32 port);
u32 in32(u32 port);

u32 load_eflags(void);
u32 load_cr0(void);
void load_gdtr(u32 limit, u32 addr);
void load_idtr(u32 limit, u32 addr);
void store_eflags(u32 eflags);
void store_cr0(u32 cr0);
void set_err(char*);

extern char EmptyStr[1];
extern u8 font[218592];
static prtFunc **FUNCS=(prtFunc**)0x7E00;
static void delay (u32 times){
	for(u32 i=1;i<times;i++)
		asm_nop();
	return;
}


#endif