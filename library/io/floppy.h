#ifndef _SYM_OS_FLOPPY_H_
#define _SYM_OS_FLOPPY_H_

#include "../func.h"

#define PORT_FLOPPY_FDOR	0x03F2
#define PORT_FLOPPY_FDCS	0x03F4
#define PORT_FLOPPY_FDCD	0x03F5
#define PORT_FLOPPY_FDIC	0x03F7

#define FDCS_READY	0x80
#define FDCS_DIRCT	0x40
#define FDCS_DMAST	0x20
#define FDCS_CMBSY	0x10
#define FDCS_DDBSY	0x08
#define FDCS_DCBSY	0x04
#define FDCS_DBBSY	0x02
#define FDCS_DABSY	0x01
#define DMA_READ	0x46
#define DMA_WRITE	0x4A
#define FD_SPECIFY		0x03
#define FD_RECALIBRATE	0x07
#define FD_SENSEI		0x08
#define FD_SEEK			0x0F
#define FD_WRITE		0xC5
#define FD_READ			0xE6

typedef struct {
	u32 size, sect, head, track, stretch;
	u8 gap,rate,spec1;
}floppy_struct;

static floppy_struct floppy_type={2880,18,2,80,0,0x1B,0x00,0xCF};
static u32 current_dev = 0;

static void reset_Floppy (void){
	out8(PORT_FLOPPY_FDOR,0x08);
	delay(128);
	out8(PORT_FLOPPY_FDOR,0x0c);
	out8(PORT_FLOPPY_FDIC,0x00);
	return;
}

static void commd_Floppy (u8 cmd){
	int cnt;u8 sts;
	for(cnt=0;cnt<10240;cnt++){
		sts=in8(PORT_FLOPPY_FDCS);
		sts&=(FDCS_READY|FDCS_DIRCT);
		if(sts==FDCS_READY){
			out8(PORT_FLOPPY_FDCD,cmd);
			return;
		}else asm_nop();
	}
	set_err("Can not command the floppy drive!");
	return;
}

static void init_Floppy(void){
	reset_Floppy();
	delay(128);
	commd_Floppy(FD_SPECIFY);
	commd_Floppy(0xcf);
	commd_Floppy(0x06);
	return;
}

static void sDMA_Floppy(u8 *buf, u8 cmd){
    u32 addr = (u32)buf;
    asm_cli();
    /* mask DMA 2 */
    out8(10,4|2);
    /* output command byte. I don't know why, but everyone (minix, */
    /* sanches & canton) output this twice, first to 12 then to 11 */
    out8(12,(cmd == FD_READ)?DMA_READ:DMA_WRITE);
    /* 8 low bits of addr */
    out8(4,addr),addr >>= 8;
    /* bits 8-15 of addr */
    out8(4,addr),addr >>= 8;
    /* bits 16-19 of addr */
    out8(0x81,addr);
    /* low 8 bits of count-1 (1024-1=0x3ff) */
    out8(5,0xff);
    /* high 8 bits of count-1 */
    out8(5,3);
    /* activate DMA 2 */
    out8(10,0|2);
    asm_sti();
    return;
}

static void read_Floppy(u32 sectNo, u8 *buf){
	u32 head, track, block, sector, seek_track;
	if(buf==NULL)return;
	if (sectNo >= (floppy_type.head * floppy_type.track * floppy_type.sect))
		return set_err(NULL);
	sector		=	sectNo	%	floppy_type.sect	+	1;
	block		=	sectNo	/	floppy_type.sect;
	track		=	block	/	floppy_type.head;
	head		=	block	%	floppy_type.head;
	seek_track	=	track	<<	floppy_type.stretch;
	commd_Floppy(FD_RECALIBRATE);
	commd_Floppy(current_dev);
	commd_Floppy(FD_SEEK);
	commd_Floppy(current_dev);
	commd_Floppy(seek_track);
	sDMA_Floppy(buf, FD_READ);
	commd_Floppy(FD_READ);			// command
	commd_Floppy(current_dev);		// driver no.
	commd_Floppy(track);			// track no.
	commd_Floppy(head);				// head
	commd_Floppy(sector);			// start sector
	commd_Floppy(2);				// sector size = 512
	commd_Floppy(floppy_type.sect);	// Max sector
	commd_Floppy(floppy_type.gap);	// sector gap
	commd_Floppy(0xFF);				// sector size (0xff when n!=0 ?)
	return;
}

#endif