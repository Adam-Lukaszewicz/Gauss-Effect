.data
maska1 db 1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 0, 0 
shuffle1 db 0, 3, 6, 9, 12, 15, 1, 4, 2, 5, 8, 11, 7, 10, 13, 14, 19, 22, 25, 28, 24, 27, 18, 21, 16, 17, 20, 23, 26, 29, 30, 31
swap1 dd 0, 1, 5, 4, 3, 2, 6, 7
swap2 dd 0, 1, 2, 4, 3, 5, 6, 7
swap3 dd 0, 1, 2, 3, 6, 5, 4, 7
swap4 dd 0, 4, 2, 3, 1, 5, 6, 7
shuffle2 db 0, 1, 2, 3, 4, 5, 10, 11, 8, 9, 6, 7, 12, 13, 14, 15, 0, 1, 2, 8, 4, 5, 6, 7, 3, 9, 10, 11, 12, 13, 14, 15
br db 1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 1, 4, 6, 4, 1, 0
m db 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 0
nm db 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0
g db 0, 0, 0, 0, 0, 1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 0
sh db 0, 3, 6, 9, 12, 1, 4, 7, 10, 13, 2, 5, 8, 11, 14, 16
three db 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
MOVDQU XMM5, XMMWORD PTR [three]
MOVDQU XMM6, XMMWORD PTR [br]
MOVDQU XMM7, XMMWORD PTR [g]
MOVDQU XMM8, XMMWORD PTR [m]
MOVDQU XMM9, XMMWORD PTR [nm]
MOVDQU XMM10, XMMWORD PTR [sh]
;VMOVDQU YMM5, YMMWORD PTR [maska1]
;vmovdqu YMM1, YMMWORD PTR [shuffle1]
;VMOVDQU YMM2, YMMWORD PTR [shuffle2]
;VMOVDQU YMM3, YMMWORD PTR [swap1]
;VMOVDQU YMM4, YMMWORD PTR [swap2]
;VMOVDQU YMM6, YMMWORD PTR [swap3]
;VMOVDQU YMM7, YMMWORD PTR [swap4]
;ADD R8, 4 ;Dodane 4 (dwa piksele buforu z kazdej strony wiersza)
MOV R9, RCX
MOV RSI, R8
MOV RDI, R8
;ADD RDX, 1
IMUL RSI, RSI, 6
IMUL RDI, RDI, 3
HORIZONTAL:
	CMP RCX, RDX
	JGE TRANSITION
	;Loading
	;VMOVDQU YMM0, YMMWORD PTR [RCX-6] ;Load 32 bytes starting from two pixels behind current pixel
	;VPSHUFB YMM0, YMM0, YMM1	;First shuffle - sets up majority of the bytes and prepares key ones for permute
	;VPERMD YMM0, YMM3, YMM0		;Permute of bytes between halves
	;VPSHUFB YMM0, YMM0, YMM2	;Second shuffle - orders the bytes in the lower half, swaps the fifth misaligned byte
	;VPERMD YMM0, YMM4, YMM0		;Last permute - swaps the dword with the last byte into lower half, finishing the loading process
	;Mulitplying
	;VPMADDUBSW YMM0, YMM0, YMM5	;All bytes get multiplied by the current mask, leaving only the ones relevnant to current pixel
	;VPERMD YMM0, YMM6, YMM0		;First permute - to setup
	;VPHADDW YMM0, YMM0, YMM0
	;VPHADDW YMM0, YMM0, YMM0
	;VPERMD YMM0, YMM7, YMM0
	;VPSRLW XMM0, XMM0, 4
	;PEXTRW RAX, XMM0, 0
	;MOV [RCX], AL
	;PEXTRW RAX, XMM0, 1
	;MOV [RCX+1], AL
	;PEXTRW RAX, XMM0, 2
	;MOV [RCX+2], AL
	
	MOVDQU XMM0, XMMWORD PTR [RCX-6] ;Load 16 bytes starting from two pixels behind the current pixel
	PSHUFB XMM0, XMM10
	VPAND XMM1, XMM0, XMM8
	VPAND XMM2, XMM0, XMM9
	PMADDUBSW XMM1, XMM6
	PMADDUBSW XMM2, XMM7
	PHADDW XMM1, XMM1
	PHADDW XMM1, XMM1
	PSRLDQ XMM2, 4
	PHADDW XMM2, XMM2
	PHADDW XMM2, XMM2
	PEXTRW RAX, XMM1, 0
	SHR RAX, 4
	MOV [RCX], AL
	PEXTRW RAX, XMM2, 0
	SHR RAX, 4
	MOV [RCX+1], AL
	PEXTRW RAX, XMM1, 1
	SHR RAX, 4
	MOV [RCX+2], AL
	ADD RCX, 3
	JMP HORIZONTAL
TRANSITION:
	MOV RCX, R9
	;SUB RDX, 1
VERTICAL:
	CMP RCX, RDX
	JGE OVER
	MOV R15, RCX
	SUB R15, RSI
	MOV R14, RCX
	SUB R14, RDI
;	PUSH BYTE [R15]
	MOVD XMM0, DWORD PTR [R15]
	MOVD XMM1, DWORD PTR [R14]
	MOVD XMM2, DWORD PTR [RCX]
	MOVD XMM3, DWORD PTR [RCX+RDI]
	MOVD XMM4, DWORD PTR [RCX+RSI]
	PAND XMM0, XMM5
	PAND XMM1, XMM5
	PAND XMM2, XMM5
	PAND XMM3, XMM5
	PAND XMM4, XMM5
	PSLLDQ XMM1, 3
	PSLLDQ XMM2, 6
	PSLLDQ XMM3, 9
	PSLLDQ XMM4, 12
	PADDB XMM0, XMM1
	PADDB XMM0, XMM2
	PADDB XMM0, XMM3
	PADDB XMM0, XMM4
	;movzx  eax, byte PTR [RCX]   ; break the
	;mov    ah,  byte PTR [RCX+RDI]
	;shl    eax, 16         ; partial-reg merge is pretty cheap on SnB/IvB, but very slow on Intel CPUs before Sandybridge.  AMD has no penalty, just (true in this case) dependencies
	;SUB RCX, RSI
	;mov    al,  byte PTR [RCX]
	;ADD RCX, RDI
	;mov    ah,  byte PTR [RCX]
	;ADD RCX, RDI
	;MOVZX R10, BYTE PTR [R15]
;	MOV RAX, R10
	
	;MOVZX R10, BYTE PTR [R14]
	;IMUL R10, R10, 4
	;ADD RAX, R10
	;MOVZX R10, BYTE PTR [RCX]
	;IMUL R10, R10, 6
	;ADD RAX, R10
	;MOVZX R10, BYTE PTR [RCX+RDI]
	;IMUL R10, R10, 4
	;ADD RAX, R10
	;MOVZX R10, BYTE PTR [RCX+RSI]
	;ADD RAX, R10
	;SHR RAX, 4
	;MOV [RCX], AL
	;XOR RAX, RAX
	PSHUFB XMM0, XMM10
	VPAND XMM1, XMM0, XMM8
	VPAND XMM2, XMM0, XMM9
	PMADDUBSW XMM1, XMM6
	PMADDUBSW XMM2, XMM7
	PHADDW XMM1, XMM1
	PHADDW XMM1, XMM1
	PSRLDQ XMM2, 4
	PHADDW XMM2, XMM2
	PHADDW XMM2, XMM2
	PEXTRW RAX, XMM1, 0
	SHR RAX, 4
	MOV [RCX], AL
	PEXTRW RAX, XMM2, 0
	SHR RAX, 4
	MOV [RCX+1], AL
	PEXTRW RAX, XMM1, 1
	SHR RAX, 4
	MOV [RCX+2], AL
	ADD RCX, 3
	JMP VERTICAL
OVER:
	mov RAX, 1
	ret
gaussTrfAsm endp
END