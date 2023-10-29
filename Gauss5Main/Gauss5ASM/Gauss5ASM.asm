.code
basicTrfAsm PROC
;RCX ma adres pocz�tku, RDX ma adres ko�ca zakresu
;RCX s�u�y jako "iterator"
;SIL (8-bitowa cz�� RSI) to bufor
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
end
