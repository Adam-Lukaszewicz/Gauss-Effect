.code
basicTrfAsm PROC
;RCX ma adres pocz¹tku, RDX ma adres koñca zakresu
;RCX s³u¿y jako "iterator"
;SIL (8-bitowa czêœæ RSI) to bufor
XOR RAX, RAX ;Zerowanie akumulatora
MYLOOP:
	mov SIL, BYTE PTR [RCX]
	add SIL, 100
	MOV [RCX], SIL
	CMP RCX, RDX
	JE OVER
	INC RCX
	JMP MYLOOP
OVER:
	mov RAX, 1
	ret
basicTrfAsm endp
gaussTrfAsm PROC
;RCX ma adres pocz¹tku, RDX ma adres koñca zakresu
;RCX s³u¿y jako "iterator"
;R8 przechowuje szerokoœæ obrazu
;SIL (8-bitowa czêœæ RSI) to bufor
XOR RAX, RAX ;Zerowanie akumulatora
ADD R8, 4
MOV R9, RCX
MOV RSI, R8
MOV RDI, R8
IMUL RSI, 6
IMUL RDI, 3
HORIZONTAL:
	MOVZX R10, BYTE PTR [RCX - 6]
	MOV R11, [RCX-6]
	MOV RAX, R10
	MOVZX R10, BYTE PTR [RCX - 3]
	MOV R11, [RCX-3]
	IMUL R10, 4
	ADD RAX, R10
	MOVZX R10, BYTE PTR [RCX]
	MOV R11, [RCX]
	IMUL R10, 6
	ADD RAX, R10
	MOVZX R10, BYTE PTR [RCX + 6]
	MOV R11, [RCX+3]
	IMUL R10, 4
	ADD RAX, R10
	MOVZX R10, BYTE PTR [RCX + 12]
	MOV R11, [RCX+6]
	ADD RAX, R10
	SHR RAX, 4
	MOV [RCX], RAX
	CMP RCX, RDX
	JE OVER
	INC RCX
	JMP HORIZONTAL
TRANSITION:
	MOV RCX, R9
VERTICAL:
	MOV R15, R9
	SUB R15, RSI
	MOVZX R10, BYTE PTR [R15]
	MOV R15, R9
	SUB R15, RDI
	MOVZX R11, BYTE PTR [R15]
	MOVZX R12, BYTE PTR [R9]
	MOVZX R13, BYTE PTR [R9+RDI]
	MOVZX R14, BYTE PTR [R9+RSI]
	IMUL R11, 4
	IMUL R12, 6
	IMUL R13, 4
	ADD RAX, R10
	ADD RAX, R11
	ADD RAX, R12
	ADD RAX, R13
	ADD RAX, R14
	SHR RAX, 4
	MOV [R9], RAX
	XOR RAX, RAX
	CMP R9, RDX
	JE OVER
	INC R9
	JMP VERTICAL
OVER:
	mov RAX, 1
	ret
gaussTrfAsm endp
end
