; Symbol OS
; DiannaoJun

; %define DBG
CYLS   EQU   35               ; 读取的柱面数量cylinders
SCTR   EQU   18               ; 扇区数量=18
%ifdef DBG
  ORG   0x100                 ; 装载地址(DOS)
%else
  ORG   0x7c00                ; 装载地址(MBR)
%endif

; 用于SFS格式的软盘
  JMP   start             ; 跳转指令
  NOP
  DW    512               ; 每个扇区 (sector) 的字节数 (必须为512字节)
  DB    1                 ; 每个簇 (cluster) 的扇区数 (必须为1个扇区)
  DW    2880              ; 磁盘的扇区总数,若为0则使用下一行记录
  DD    2880              ; 大容量扇区总数,若上一行记录的值为0则使用本行记录扇区数
  DB    1                 ; 分区数量
  DB    0xf0              ; 磁盘的种类 (本项目中设为0xf0代表1.44MB的软盘)
  DD    0xffffffff        ; 磁盘序号 
  DB    "SBR     "        ; 系统引导扇区格式

  DB    "SFS     "        ; 文件系统类型 (8字节)
  DB    1                 ; 每个块 (block) 的扇区数 (为1个扇区)
  DW    0                 ; 开始扇区,若为0则使用下一行记录
  DD    0                 ; 开始扇区,若上一行记录的值为0则使用本行记录
  DW    2880              ; 分区的扇区总数,若为0则使用下一行记录
  DD    2880              ; 大分区扇区总数,若上一行记录的值为0则使用本行记录扇区数
  DW    1                 ; FAT前的预留扇区数 (包含boot扇区)
  DB    1                 ; FAT表的数量,通常为2
  DW    224               ; 根目录文件的最大值 (一般设为224项)
  DW    24                ; 备份引导扇区
  DB    "SYMBOL OS  "     ; 卷标 (11字节)

  DB    0x00
  NOP


start:
  MOV   AX, 0             ; 初始化寄存器
  MOV   SS, AX
  MOV   SP, 0x7c00
  MOV   DS, AX

system:
  MOV   AH,0
  MOV	  AL,3
  INT   0x10
  MOV   AH,2
  MOV   BH,0
  MOV   DH,0
  MOV   DL,0
  INT   0x10
  MOV   AH,6
  MOV   AL,0
  INT   0x10
  MOV	[dsk],dl
; 读取硬盘
  MOV   AX, 0x0800
  MOV   ES, AX
  MOV   CH, 0             ; 柱面0
  MOV   DH, 0             ; 磁头0
  MOV   CL, 1             ; 扇区1
  CALL  loop0
  JMP   0xCC00

loop0:
  MOV   SI, 0             ; 记录失败次数
retry:
  MOV   AH, 0x02          ; 读盘
  MOV   AL, 1             ; 1个扇区
  MOV   BX, 0
  MOV   DL, [dsk]          ; 驱动器
  INT   0x13              ; 调用BIOS
  JNC   next              ; 没出错跳next

  ADD   SI, 1             ; 失败++
  CMP   SI, 5             ; 失败5次?
  JAE   error             ; 失败5次跳error
  MOV   AH, 0x00
  MOV   DL, [dsk]         ; 驱动器
  INT   0x13              ; 重置驱动器
  JMP   retry

next:
  MOV   AX, ES            ; 把内存地址后移512
  ADD   AX, 0x0020
  MOV   ES, AX            ; ES += 512

  ; 扇区 1~18
  ADD   CL, 1             ; 扇区++
  CMP   CL, SCTR          ; 扇区到18?
  JBE   loop0             ; 小于等于18扇区跳readloop

  MOV   CL, 1             ; 到扇区1
  ; 磁头 正0~反1
  ADD   DH, 1
  CMP   DH, 2
  JB    loop0             ; 磁头未到2跳readloop

  MOV   DH, 0
  ; 柱面 0 ~ 79
  ADD   CH, 1
  CMP   CH, CYLS
  JB    loop0             ; 读取指定数量的柱面,未到CYLS跳readloop

; 读取完毕,跳kernel.sys执行
  MOV   [0x0ff0], CH      ; 记IPL读多远
  RET

rtn:
  RET

error:
  MOV   SI, errmsg
  CALL  putloop
  L0: ;hlt
  nop
  jmp L0

putloop:
  MOV   AL, [SI]
  ADD   SI, 1             ; SI加1
  CMP   AL, 0

  JE    rtn
  MOV   AH, 0x0e          ; 显示一个文字
  MOV   BX, 15            ; 指定字符颜色
  INT   0x10              ; 调用显卡BIOS
  JMP   putloop
errmsg:
  DB    0x0a, 0x0a        ; 两个换行
  DB    "Loading System Program Error"
  DB    0x0a, 0x0d        ; 换行回车
  DB    "Please Press Ctrl+Alt+Del to Restart"
  DB    0x0a, 0x0d        ; 换行回车
  DB    "Error-code: IPL 0000-0001"
  DB    0x0a, 0x0d        ; 换行回车
  DB    0

  TIMES 0x1fd - ($ - $$)  DB 0    ; 填写0x00，直到0x001fe
  dsk	DB	0x00
  DB    0x55, 0xaa
; error code      |
; all right       0
; disk IO error   1
; screen IO error 2
; other           ff