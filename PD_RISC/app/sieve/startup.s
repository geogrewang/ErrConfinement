	.ref _main
	.global _exit
	.global __EXIT
	.global __C_Entry2
	.global _syscall_handler
   
	.tsect ".boot"
__C_Entry2:
	ldi16 r14,0x0ff0
	ldi16 r15,((_main)&0xffff)
	zxt16 r15, r15		; remove potential sign-extend bits from ldi16
	call r15
	nop
	nop
	nop
	nop
__EXIT:					; for hw simulator
_exit:					; for sw simulator
	b __EXIT
	nop
	nop
	nop
	nop
	nop
	.end
