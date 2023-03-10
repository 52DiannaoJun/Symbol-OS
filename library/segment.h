#ifndef _SYM_OS_SEGMENT_H_
#define _SYM_OS_SEGMENT_H_

#include "./int.h"

#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

struct SegmentDescriptor {
  short limit_low, base_low;
  char base_mid, access_right;
  char limit_high, base_high;
};

struct GateDescriptor {
  short offset_low, selector;
  char dw_count, access_right;
  short offset_high;
};

static void set_gatedesc(struct GateDescriptor *gd, int offset, int selector, int ar);
static void set_segmdesc(struct SegmentDescriptor *sd, unsigned int limit, int base, int ar);

static void init_gdtidt(void) {
  struct SegmentDescriptor *gdt = (struct SegmentDescriptor *)ADR_GDT;
  struct GateDescriptor *idt = (struct GateDescriptor *)ADR_IDT;

  for (int i = 0; i < 8192; i++) {
    set_segmdesc(gdt + i, 0, 0, 0);
  }

  set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW);
  set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
  load_gdtr(LIMIT_GDT, ADR_GDT);

  for (int i = 0; i < 256; i++) {
    set_gatedesc(idt + i, 0, 0, 0);
  }
  load_idtr(LIMIT_IDT, ADR_IDT);
  
  set_gatedesc(idt + 0x03, (int) int0d_asm, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x20, (int) int20_asm, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x21, (int) int21_asm, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x27, (int) int27_asm, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x2c, (int) int2c_asm, 2 * 8, AR_INTGATE32);
  set_gatedesc(idt + 0x40, (int) int40_asm, 2 * 8, AR_INTGATE32);
  return;
}

static void set_segmdesc(struct SegmentDescriptor *sd, unsigned int limit, int base, int ar) {
  if (limit > 0xfffff) {
    ar |= 0x8000; // G_bit = 1
    limit /= 0x1000;
  }

  sd->limit_low = limit & 0xffff;
  sd->base_low = base & 0xffff;
  sd->base_mid = (base >> 16) & 0xff;
  sd->access_right = ar & 0xff;
  sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) | 0xf0);
  sd->base_high = (base >> 24) & 0xff;
  return;
}

static void set_gatedesc(struct GateDescriptor *gd, int offset, int selector, int ar) {
  gd->offset_low = offset & 0xffff;
  gd->selector = selector;
  gd->dw_count = (ar >> 8) & 0xff;
  gd->access_right = ar & 0xff;
  gd->offset_high = (offset >> 16) & 0xffff;
  return;
}

#endif