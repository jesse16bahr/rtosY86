

YKEnterMutex:
	push bp
	mov  bp, sp
	cli
	pop bp
	ret

YKExitMutex:
	push bp
	mov  bp, sp
	sti
	pop bp
	ret


	
