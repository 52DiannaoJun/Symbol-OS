#ifndef _SYM_OS_GRAPH_H_
#define _SYM_OS_GRAPH_H_
#include "./func.h"

typedef struct {
  char cyls, leds, vmode, reserve;
  u16 scrnx, scrny;
  u8 *vram;
} ScrnInfo;
static void init_palette(void);
static void set_palette(int start, int end, u8 *rgb);
static void box_fill(ScrnInfo*scnini, u8 c, u16 x0, u16 y0, u16 x1, u16 y1);
static void init_screen(ScrnInfo*scnini);
static void put_font8(ScrnInfo*scnini, u16 x, u16 y, u8 c, u8 *font);
static void put_fonts8_asc(ScrnInfo*scnini, u16 x, u16 y, u8 c, char *s);
static void put_block8(u8 *vram, u16 vxsize, u16 pxsize, u16 pysize, u16 px0, u16 py0, u8 *buf, u16 bxsize);
static short putstr(ScrnInfo*scnini, short x, u16 y, u8 c, char *s) ;

#define RGB_000000 0x00
#define RGB_FF0000 0x01
#define RGB_00FF00 0x02
#define RGB_FFFF00 0x03
#define RGB_0000FF 0x04
#define RGB_FF00FF 0x05
#define RGB_00FFFF 0x06
#define RGB_CFCFCF 0x07
#define RGB_FFFFFF 0x08
#define RGB_7F0000 0x09
#define RGB_007F00 0x0A
#define RGB_7F7F00 0x0B
#define RGB_00007F 0x0C
#define RGB_7F007F 0x0D
#define RGB_007F7F 0x0E
#define RGB_7F7F7F 0x0F
#define RGB_CF1F7F 0x10
#define RGB_FF7F00 0x11
#define RGB_CF1F1F 0x12

void init_palette(void) {
  #define CLCS (19+1)
  static u8 table_rgb[CLCS * 3] = {
      0x00, 0x00, 0x00, // 黑色
      0xff, 0x00, 0x00, // 亮红色
      0x00, 0xff, 0x00, // 亮绿色
      0xff, 0xff, 0x00, // 亮黄色
      0x00, 0x00, 0xff, // 亮蓝色
      0xff, 0x00, 0xff, // 亮紫色
      0x00, 0xff, 0xff, // 浅青色
      0xcf, 0xcf, 0xcf, // 亮灰色
      0xff, 0xff, 0xff, // 白色
      0x7f, 0x00, 0x00, // 暗红色
      0x00, 0x7f, 0x00, // 暗绿色
      0x7f, 0x7f, 0x00, // 暗黄色
      0x00, 0x00, 0x7f, // 暗蓝色
      0x7f, 0x00, 0x7f, // 暗紫色
      0x00, 0x7f, 0x7f, // 暗青色
      0x7f, 0x7f, 0x7f, // 暗灰色
      0xcf, 0x1f, 0x7f, // 粉色
      0xff, 0x7f, 0x00, // 橙色
      0xcf, 0x1f, 0x1f, // 棕色
      0x00, 0x00, 0x00
  };

  set_palette(0, CLCS, table_rgb);
  return;
}

static void set_palette(int start, int end, u8 *rgb) {
  int eflags = load_eflags(); // 记录标志

  asm_cli(); // 禁止中断

  out8(0x03c8, start);
  for (int i = start; i <= end; i++) {
    out8(0x03c9, rgb[0] / 4);
    out8(0x03c9, rgb[1] / 4);
    out8(0x03c9, rgb[2] / 4);
    rgb += 3;
  }

  store_eflags(eflags);
  return;
}

static void box_fill(ScrnInfo*scnini, u8 c, u16 x0, u16 y0, u16 x1, u16 y1) {
  for (u16 y = y0; y <= y1; y++) {
    for (u16 x = x0; x <= x1; x++) {
      scnini->vram[y * scnini->scrnx + x] = c;
    }
  }
  return;
}

static void init_screen(ScrnInfo*scnini){
  extern u8 icon[256][256];
  static char TITLE[]="Symbol OS Version: 0.2 Alpha";
  u16 i,j;
  box_fill(scnini, RGB_00FFFF, 0, 0, scnini->scrnx - 1, scnini->scrny - 1);
  for(i=0;i<256;i++)for(j=0;j<256;j++)
	  scnini->vram[(i-128+scnini->scrny/2)*(scnini->scrnx)+(j-128+scnini->scrnx/2)]=icon[i][j];
  for(i=0;i<256;i++)
    scnini->vram[(scnini->scrny/2+127)*scnini->scrnx+scnini->scrnx/2+i]=i;
  box_fill(scnini, RGB_000000 , scnini->scrnx/2, scnini->scrny/2+128, scnini->scrnx/2+128, scnini->scrny/2+132);
  box_fill(scnini, RGB_7F7F7F , scnini->scrnx/2, scnini->scrny/2+128, scnini->scrnx/2+112, scnini->scrny/2+132);
  box_fill(scnini, RGB_CFCFCF , scnini->scrnx/2, scnini->scrny/2+128, scnini->scrnx/2+96 , scnini->scrny/2+132);
  box_fill(scnini, RGB_FFFFFF , scnini->scrnx/2, scnini->scrny/2+128, scnini->scrnx/2+80 , scnini->scrny/2+132);
  box_fill(scnini, RGB_FF0000 , scnini->scrnx/2, scnini->scrny/2+128, scnini->scrnx/2+64 , scnini->scrny/2+132);
  box_fill(scnini, RGB_0000FF , scnini->scrnx/2, scnini->scrny/2+133, scnini->scrnx/2+128, scnini->scrny/2+137);
  put_fonts8_asc(scnini, scnini->scrnx/2  , scnini->scrny/2+140, RGB_000000, TITLE);
  put_fonts8_asc(scnini, scnini->scrnx/2-1, scnini->scrny/2+139, RGB_FFFFFF, TITLE);
  return;
}

static void put_font8(ScrnInfo*scnini, u16 x, u16 y, u8 c, u8 *font) {
  u8 *p, d;

  for (u16 i = 0; i < 16; i++) {
    p = scnini->vram + (y + i) * scnini->scrnx + x;
    d = font[i];

    if ((d & 0x80) != 0)
      p[0] = c;
    if ((d & 0x40) != 0)
      p[1] = c;
    if ((d & 0x20) != 0)
      p[2] = c;
    if ((d & 0x10) != 0)
      p[3] = c;
    if ((d & 0x08) != 0)
      p[4] = c;
    if ((d & 0x04) != 0)
      p[5] = c;
    if ((d & 0x02) != 0)
      p[6] = c;
    if ((d & 0x01) != 0)
      p[7] = c;
  }
  return;
}

static void put_fonts8_asc(ScrnInfo*scnini, u16 x, u16 y, u8 c, char *str) {
  u32 charcode;
  u8 *s=(u8*)str;
  for (; *s != '\0'; s++) {
    if(*s=='\n')y+=16;
    else if(*s=='\r')x=-8;
    else if(*s=='\t')x+=8;
    else if(*s>0x7f&&*s<0xc0){
      charcode=*s&0x3f,charcode<<=8,charcode|=*(s+1),charcode&=0x3fff;
      put_font8(scnini, x, y, c, font + 32*charcode+2048),x+=8;
      put_font8(scnini, x, y, c, font + 32*charcode+2064),s++;
    }
    else put_font8(scnini, x, y, c, font + *s * 16);
    x += 8;
  }
  return;
}

static void put_block8(u8 *vram, u16 vxsize, u16 pxsize, u16 pysize, u16 px0, u16 py0, u8 *buf, u16 bxsize) {
  for (u16 y = 0; y < pysize; y++) {
    for (u16 x = 0; x < pxsize; x++) {
      vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
    }
  }
  return;
}

static void cls(ScrnInfo*scnini){
  u16 x,y;
  for(y=16;y<scnini->scrny;y++)
    for(x=0;x<scnini->scrnx;x++)
      scnini->vram[y*scnini->scrnx+x]=0;
  return;
}

static void swim_line(ScrnInfo*scnini){
	u16 i,j;
	for(i=16;i<scnini->scrny;i++)for(j=0;j<scnini->scrnx;j++)
		scnini->vram[(i-16)*scnini->scrnx+j]=scnini->vram[i*scnini->scrnx+j];
	for(i=0;i<16;i++)for(j=0;j<scnini->scrnx;j++)
		scnini->vram[(scnini->scrny-i-1)*scnini->scrnx+j]=0;
	return;
}

static short putstr(ScrnInfo*scnini, short x, u16 y, u8 c, char *str) {
  u32 charcode;
  u8 *s=(u8*)str;
  for (; *s != '\0'; s++) {
    if(x>=scnini->scrnx)swim_line(scnini),x=-8;
    if(*s=='\n')swim_line(scnini);
    else if(*s=='\r')x=-8;
    else if(*s=='\t')x+=8;
    else if(*s>0x7f&&*s<0xc0){
      charcode=*s&0x3f,charcode<<=8,charcode|=*(s+1),charcode&=0x3fff;
      put_font8(scnini, x, y, c, font + 32*charcode+2048),x+=8;
      put_font8(scnini, x, y, c, font + 32*charcode+2064),s++;
    }
    else if(*s<0x80){
      put_font8(scnini, x, y, c, font + *s * 16);
    }
    else put_font8(scnini, x, y, c, font + 63 * 16);
    x += 8;
  }
  return x;
}

#endif