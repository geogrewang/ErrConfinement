	.text
	.globl	_main
	.type	_main,@function
_main:                                  ;; @main
;; BB#0:
	sw  R13 , R14 , 0
	mov  R13 , R14
	add  R14 , -20
	ldi  R0 , 0
	ldih R1, ((__.str >> 16) & 0xffff)
	or16 R1, ((__.str) & 0xffff)
	sw  R15 , R13 , -4
	sw  R0 , R13 , -12
	call  _printf
	sw  R1 , R14 , 4
	lw  R15 , R13 , -4
	mov  R14 , R13
	lw  R13 , R14 , 0
	b  r15
	ldi  R0 , 0
_tmp0:
	.size	_main, _tmp0-_main

	.type	__.str,@object          ;; @.str
	.data
__.str:
	.asciz	 "Hello World of Programmable Accelerators!\n"
	.size	__.str, 43


