[BITS 32]
GLOBAL	int20_asm,	int21_asm,	int27_asm,	int2c_asm
GLOBAL  int40_asm,	int13_asm,	int0d_asm
EXTERN	int20_c,	int21_c,	int27_c,	int2c_c
EXTERN  int40_c,	int13_c,	int0d_c
tmp0	DD 0
tmp1	DD 0
tmp2	DD 0

int21_asm:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	int21_c
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD
int27_asm:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	int27_c
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD
int2c_asm:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	int2c_c
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD
int20_asm:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	int20_c
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD
int40_asm:
	STI
	PUSHAD	
	PUSHAD	
	CALL	int40_c
	ADD		ESP,32
	POPAD
	IRETD
int13_asm:
	STI
	PUSHAD	
	PUSHAD	
	CALL	int13_c
	ADD		ESP,32
	POPAD
	IRETD
int0d_asm:
	STI
	PUSHAD	
	PUSHAD	
	CALL	int0d_c
	ADD		ESP,32
	POPAD
	IRETD
