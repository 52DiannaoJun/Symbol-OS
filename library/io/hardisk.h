#ifndef _SYM_OS_HARDISK_H_
#define _SYM_OS_HARDISK_H_

#define PORT_HARDISK_DATA	0x1f0
#define PORT_HARDISK_ZERO	0x1f1
#define PORT_HARDISK_CNTS	0x1f2
#define PORT_HARDISK_VAL0	0x1f3
#define PORT_HARDISK_VAL1	0x1f4
#define PORT_HARDISK_VAL2	0x1f5
#define PORT_HARDISK_INFO	0x1f6
#define PORT_HARDISK_COMD	0x1f7

#include "../func.h"
/*static void init_Hradisk (void){
	out8(PIC_M_DATA,0xF8);
	out8(PIC_S_DATA,0xBF);
	return;
}*/

void read_CHS(void*dat,u16 cnts,u16 sctr,u16 cyls,u16 head,u16 disk){
	u16 mov=0,i;
	if(disk>1)mov=0x80;
	head&=0x0f,disk&=0x10,disk<<=4;
	disk|=head|0xa0,head=cyls>>8;
	out8(PORT_HARDISK_ZERO-mov,0x00);
	out8(PORT_HARDISK_CNTS-mov,cnts);
	out8(PORT_HARDISK_VAL0-mov,sctr);
	out8(PORT_HARDISK_VAL1-mov,head);
	out8(PORT_HARDISK_VAL2-mov,cyls);
	out8(PORT_HARDISK_INFO-mov,disk);
	out8(PORT_HARDISK_COMD-mov,0x20);
	for(i=0,cnts*=256;i<cnts;i++){
		while(in8(PORT_HARDISK_COMD-mov)&0x10)delay(128);
		((u16*)dat)[i]=in16(PORT_HARDISK_DATA-mov);
	}
}
void write_CHS(void*dat,u16 cnts,u16 sctr,u16 cyls,u16 head,u16 disk){
	u16 mov=0,i;
	if(disk>1)mov=0x80;
	head&=0x0f,disk&=0x10,disk<<=4;
	disk|=head|0xa0,head=cyls>>8;
	out8(PORT_HARDISK_ZERO-mov,0x00);
	out8(PORT_HARDISK_CNTS-mov,cnts);
	out8(PORT_HARDISK_VAL0-mov,sctr);
	out8(PORT_HARDISK_VAL1-mov,head);
	out8(PORT_HARDISK_VAL2-mov,cyls);
	out8(PORT_HARDISK_INFO-mov,disk);
	out8(PORT_HARDISK_COMD-mov,0x30);
	for(i=0,cnts*=256;i<cnts;i++){
		while(in8(PORT_HARDISK_COMD-mov)&0x10)delay(128);
		out16(PORT_HARDISK_DATA-mov,((u16*)dat)[i]);
	}
}
void read_LBA28 (void*dat,u8 cnts,u32 sctr,u8 disk){
	u16 mov=0,i;
	if(disk>1)mov=0x80;
	disk%=2,disk<<=4,disk&=0x10,disk|=0xe0,disk|=cnts>>24;
	out8(PORT_HARDISK_ZERO-mov,0x00);
	out8(PORT_HARDISK_CNTS-mov,cnts);
	out8(PORT_HARDISK_VAL0-mov,sctr>>0x00);
	out8(PORT_HARDISK_VAL1-mov,sctr>>0x08);
	out8(PORT_HARDISK_VAL2-mov,sctr>>0x10);
	out8(PORT_HARDISK_INFO-mov,disk);
	out8(PORT_HARDISK_COMD-mov,0x20);
	for(i=0,cnts*=256;i<cnts;i++){
		while(in8(PORT_HARDISK_COMD-mov)&0x10)delay(128);
		((u16*)dat)[i]=in16(PORT_HARDISK_DATA-mov);
	}
}
void write_LBA28(void*dat,u8 cnts,u32 sctr,u8 disk){
	u16 mov=0,i;
	if(disk>1)mov=0x80;
	disk%=2,disk<<=4,disk&=0x10,disk|=0xe0,disk|=cnts>>24;
	out8(PORT_HARDISK_ZERO-mov,0x00);
	out8(PORT_HARDISK_CNTS-mov,cnts);
	out8(PORT_HARDISK_VAL0-mov,sctr>>0x00);
	out8(PORT_HARDISK_VAL1-mov,sctr>>0x08);
	out8(PORT_HARDISK_VAL2-mov,sctr>>0x10);
	out8(PORT_HARDISK_INFO-mov,disk);
	out8(PORT_HARDISK_COMD-mov,0x30);
	for(i=0,cnts*=256;i<cnts;i++){
		while(in8(PORT_HARDISK_COMD-mov)&0x10)delay(128);
		out16(PORT_HARDISK_DATA-mov,((u16*)dat)[i]);
	}
}
void read_LBA48 (void*dat,u16 cnts,u64 sctr,u8 disk){
	u16 mov=0,i;
	if(disk>1)mov=0x80;
	disk%=2,disk<<=4,disk&=0x10,disk|=0x40;
	out8(PORT_HARDISK_ZERO-mov,0x00);
	out8(PORT_HARDISK_CNTS-mov,cnts>>8);
	out8(PORT_HARDISK_CNTS-mov,cnts>>0);
	out8(PORT_HARDISK_VAL0-mov,sctr>>24);
	out8(PORT_HARDISK_VAL0-mov,sctr>>0);
	out8(PORT_HARDISK_VAL1-mov,sctr>>32);
	out8(PORT_HARDISK_VAL1-mov,sctr>>8);
	out8(PORT_HARDISK_VAL2-mov,sctr>>40);
	out8(PORT_HARDISK_VAL2-mov,sctr>>16);
	out8(PORT_HARDISK_INFO-mov,disk);
	out8(PORT_HARDISK_COMD-mov,0x24);
	for(i=0,cnts*=256;i<cnts;i++){
		while(in8(PORT_HARDISK_COMD-mov)&0x10)delay(128);
		((u16*)dat)[i]=in16(PORT_HARDISK_DATA-mov);
	}
}
void write_LBA48 (void*dat,u16 cnts,u64 sctr,u8 disk){
	u16 mov=0,i;
	if(disk>1)mov=0x80;
	disk%=2,disk<<=4,disk&=0x10,disk|=0x40;
	out8(PORT_HARDISK_ZERO-mov,0x00);
	out8(PORT_HARDISK_CNTS-mov,cnts>>8);
	out8(PORT_HARDISK_CNTS-mov,cnts>>0);
	out8(PORT_HARDISK_VAL0-mov,sctr>>24);
	out8(PORT_HARDISK_VAL0-mov,sctr>>0);
	out8(PORT_HARDISK_VAL1-mov,sctr>>32);
	out8(PORT_HARDISK_VAL1-mov,sctr>>8);
	out8(PORT_HARDISK_VAL2-mov,sctr>>40);
	out8(PORT_HARDISK_VAL2-mov,sctr>>16);
	out8(PORT_HARDISK_INFO-mov,disk);
	out8(PORT_HARDISK_COMD-mov,0x34);
	for(i=0,cnts*=256;i<cnts;i++){
		while(in8(PORT_HARDISK_COMD-mov)&0x10)delay(128);
		out16(PORT_HARDISK_DATA-mov,((u16*)dat)[i]);
	}
}
#endif