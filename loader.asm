; Symbol OS
; DiannaoJun

VBEMODE EQU   0x105
; 0x100:  640 *  400  256
; 0x101:  640 *  480  256
; 0x103:  800 *  600  256
; 0x105: 1024 *  768  256
; 0x107: 1280 * 1024  256

BOTPAK  EQU   0x00280000  ; 加载Kernel
DSKCAC  EQU   0x00100000  ; 磁盘缓存的位置
DSKCAC0 EQU   0x00008000  ; 实模式磁盘缓存的位置

; 有关BOOT_INFO
CYLS    EQU   0x0ff0      ; 设置启动区
LEDS    EQU   0x0ff1
VMODE   EQU   0x0ff2      ; 关于颜色数目的信息，颜色的位数
SCRNX   EQU   0x0ff4      ; 分辨率X
SCRNY   EQU   0x0ff6      ; 分辨率Y
VRAM    EQU   0x0ff8      ; 图像缓冲区的起始位置
dsk		EQU	  0x7dfd
  ; 使用linker script指定起始地址
  ORG   0xCC00            ; 程序被加载的内存地址

  [BITS 16]
JMP entry

optmsg:
  DB    " --- Boot Menu --- [ ]",0x0a,0x0d
  DB    "0) Reset",0x0a,0x0d
  DB    "1) Restart",0x0a,0x0d
  DB    "2) Shutdown",0x0a,0x0d
  DB    "3) Load the Symbol Operating System",0x0a,0x0d
  DB    "4) Load the Operating System in Disk #0",0x0a,0x0d,
  DB    "5) Load the Operating System in Floppy #1",0x0a,0x0d,0
othros:
    ; 将c盘0道0面1扇区读入到内存0:7c00处,然后跳转到0:7c00处执行程序
  mov ax, 0
  mov es, ax
  mov bx, 7c00h
  mov al, 1
  mov ch, 0
  mov cl, 1
  mov dl, [dsk]                 ; 硬盘C:80h
  mov dh, 0
  mov ah, 2
  int 13h
  mov ax, 0
  push ax
  mov ax, 7c00h
  push ax
  retf
putloop:
  MOV   AL, [SI]
  ADD   SI, 1             ; SI加1
  CMP   AL, 0
  JE    rtn
  MOV   AH, 0x0e          ; 显示一个文字
  MOV   BX, 15            ; 指定字符颜色
  INT   0x10              ; 调用显卡BIOS
  JMP   putloop
rtn:
  RET
SD:
  MOV AX, 5301H         ;5301h方法:APM实模式控制
  XOR BX, BX            ;设备ID: 0000h (=BIOS)
  INT 15H               ;15h中断
  MOV AX, 530EH         ;530Eh方法:APM设备版本
  MOV CX, 0102H         ;版本: APM v1.2
  INT 15H               ;15h号中断
  MOV AX, 5307H         ;5307h: APM电源管理
  MOV BL, 01H           ;设备ID: 0001h (=所有设备)
  MOV CX, 0003H         ;电源管理: 0003h (=关闭)
  INT 15H               ;15h中断
rst:
  JMP 00000H:7c00H
rsr:
  JMP 0FFFFH:0000H

entry:
  MOV   SI, optmsg
  CALL  putloop
  optloop:
	MOV byte [dsk],01h
    MOV AH,0x00
	INT 0x16
    MOV AH,2
    MOV BH,0
    MOV DH,0
    MOV DL,20
    INT 0x10
	MOV AH,0x0a
	MOV BH,0
	MOV CX,1
	INT 0x10
    MOV AH,2
    MOV BH,0
    MOV DH,0
    MOV DL,0
    INT 0x10
	MOV DL,[dsk]
	
	CMP AL,'0'
	JE  rst
	CMP AL,'1'
	JE  rsr
	CMP AL,'2'
	JE  SD
	CMP AL,'3'
	JE  system
	CMP AL,'5'
    JE  othros
	CMP AL,'4'
    JNE optloop
	MOV byte [dsk],80h
    JE  othros

system:
; 设置屏幕模式
  ;JMP scrn320 ;Debug
; 确定VBE是否存在
        xchg bx,bx ;DBG
  MOV   AX, 0x9000
  MOV   ES, AX
  MOV   DI, 0
  MOV   AX, 0x4f00
  INT   0x10
  CMP   AX, 0x004f
  JNE   scrn320

; 检查VBE的版本
  MOV   AX, [ES:DI+4]
  CMP   AX, 0x0200
  JB    scrn320

; 取得画面模式信息
  MOV   CX, VBEMODE
  MOV   AX, 0x4f01
  INT   0x10
  CMP   AX, 0x004f
  JNE   scrn320

; 画面模式的确认
  CMP   BYTE [ES:DI+0x19], 8
  JNE   scrn320
  CMP   BYTE [ES:DI+0x1b], 4
  JNE   scrn320
  MOV   AX, [ES:DI+0x00]
  AND   AX, 0x0080
  JZ    scrn320

; 画面模式的切换
  MOV   BX, VBEMODE+0x4000
  MOV   AX, 0x4f02
  INT   0x10
  MOV   BYTE [VMODE], 8   ; 记下画面模式
  MOV   AX, [ES:DI+0x12]
  MOV   [SCRNX], AX
  MOV   AX, [ES:DI+0x14]
  MOV   [SCRNY], AX
  MOV   EAX, [ES:DI+0x28]
  MOV   [VRAM], EAX
  JMP   keystatus

scrn320:
  MOV   AL, 0x13          ; VGA图，320*200 256彩色
  MOV   AH, 0x00
  INT   0x10
  MOV   BYTE [VMODE], 8    ; 记下画面模式
  MOV   WORD [SCRNX], 320
  MOV   WORD [SCRNY], 200
  MOV   DWORD [VRAM], 0x000a0000

keystatus:
; 用BIOS取得键盘上各种LED指示灯的状态
  MOV   AH, 0x02
  INT   0x16              ; 键盘BIOS
  MOV   [LEDS], AL

; 防止PIC接受所有中断
;   根据AT兼容机的规范初始化PIC
;   如果没有在CLI指令前执行可能会挂起
;   并继续初始化PIC
  MOV   AL, 0xff
  OUT   0x21, AL
  NOP                     ; 有些机器不能连续执行NOP指令
  OUT   0xa1, AL

  CLI

; 设置A20GATE使CPU支持1M以上的内存
  CALL  waitkbdout
  MOV   AL, 0xd1
  OUT   0x64, AL
  CALL  waitkbdout
  MOV   AL, 0xdf          ; 开启A20
  OUT   0x60, AL
  CALL  waitkbdout

; 切换到保护模式
  LGDT  [GDTR0]           ; 设置临时GDT
  MOV   EAX, CR0
  AND   EAX, 0x7fffffff   ; 禁用分页
  OR    EAX, 0x00000001   ; 开启保护模式
  MOV   CR0, EAX
  JMP   pipelineflush

pipelineflush:
  MOV   AX, 1*8           ; 写32bit段
  MOV   DS, AX
  MOV   ES, AX
  MOV   FS, AX
  MOV   GS, AX
  MOV   SS, AX

; bootpack传递
  MOV   ESI, bootpack     ; 源
  MOV   EDI, BOTPAK       ; 目标
  MOV   ECX, 512*1024/4
  CALL  memcpy

; 传输磁盘数据

; 从引导区开始

  MOV   ESI, 0x7c00       ; 源
  MOV   EDI, DSKCAC       ; 目标
  MOV   ECX, 512/4
  CALL  memcpy

; 剩余的全部
  MOV   ESI, DSKCAC0+512  ; 源
  MOV   EDI, DSKCAC+512   ; 目标
  MOV   ECX, 0
  MOV   CL, BYTE [CYLS]
  IMUL  ECX, 512*18*2/4   ; 除以4得到字节数
  SUB   ECX, 512/4        ; IPL偏移量
  CALL  memcpy

skip:
  MOV   ESP, 0xffff
  JMP   DWORD 2*8:0x00000000

waitkbdout:
  IN    AL, 0x64
  AND   AL, 0x02
  JNZ   waitkbdout        ; AND结果不为0跳转至waitkbdout
  RET

memcpy:
  MOV   EAX, [ESI]
  ADD   ESI, 4
  MOV   [EDI], EAX
  ADD   EDI, 4
  SUB   ECX, 1
  JNZ   memcpy            ; 结果不为0跳转至memcpy
  RET
; memcpy地址前缀大小

  ALIGN 16
GDT0:
  TIMES 8 DB 0x00                       ; 初始值
  DW    0xffff, 0x0000, 0x9200, 0x00cf  ; 可写的32位段寄存器
  DW    0xffff, 0x0000, 0x9a28, 0x0047  ; 可执行的文件的32位寄存器

  DW    0

GDTR0:
  DW    8*3-1
  DD    GDT0

  ALIGN 16
bootpack:
