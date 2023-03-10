  [BITS 32]

  GLOBAL asm_hlt, asm_cli, asm_sti, asm_stihlt, asm_nop
  GLOBAL in8, in16, in32
  GLOBAL out8, out16, out32
  GLOBAL load_eflags, store_eflags
  GLOBAL load_gdtr, load_idtr
  GLOBAL load_cr0, store_cr0, far_jmp
  GLOBAL set_err

  [SECTION .text]
load_gdtr:              ; void load_gdtr(int limit, int addr);
  MOV   AX, [ESP+4]     ; limit
  MOV   [ESP+6], AX
  LGDT  [ESP+6]
  RET

load_idtr:              ; void load_idtr(int limit, int addr);
  MOV   AX, [ESP+4]     ; limit
  MOV   [ESP+6], AX
  LIDT  [ESP+6]
  RET

asm_hlt:                ; void asm_hlt(void);
  HLT
  RET

asm_cli:                ; void asm_cli(void);
  CLI
  RET

asm_sti:                ; void asm_sti(void);
  STI
  RET

asm_stihlt:             ; void asm_stihlt(void);
  STI
  HLT
  RET

asm_nop:                ; void asm_nop (void);
  NOP
  RET

in8:                    ; int in8(int port);
  MOV     EDX, [ESP+4]  ; port
  MOV     EAX, 0
  IN      AL, DX
  RET

in16:                   ; int in16(int port);
  MOV     EDX, [ESP+4]  ; port
  MOV     EAX, 0
  IN      AX, DX
  RET

in32:                   ; int in32(int port);
  MOV     EDX, [ESP+4]  ; port
  IN      EAX, DX
  RET

out8:                   ; void out8(int port, int data);
  MOV     EDX, [ESP+4]  ; port
  MOV     AL, [ESP+8]   ; data
  OUT     DX, AL
  RET

out16:                  ; void out16(int port, int data);
  MOV     EDX, [ESP+4]  ; port
  MOV     AX, [ESP+8]   ; data
  OUT     DX, AX
  RET

out32:                  ; void out32(int port, int data);
  MOV     EDX, [ESP+4]  ; port
  MOV     EAX, [ESP+8]  ; data
  OUT     DX, EAX
  RET

load_eflags:            ; int load_eflags(void);
  PUSHFD
  POP     EAX
  RET

store_eflags:           ; void store_eflags(int eflags);
  MOV     EAX, [ESP+4]  ; eflags
  PUSH    EAX
  POPFD
  RET

load_cr0:               ; int load_cr0(void);
  MOV     EAX, CR0
  RET

store_cr0:              ; void store_cr0(int cr0);
  MOV     EAX, [ESP+4]  ; cr0
  MOV     CR0, EAX
  RET

far_jmp:                ; void far_jmp(int eip, int cs);
  JMP   FAR [ESP+4]
  RET

set_err:                ; void set_err(char*);
  MOV EAX, [ESP+4];
  INT 0x03
  RET