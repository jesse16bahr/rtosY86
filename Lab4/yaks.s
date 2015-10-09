

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

YKDispatcher:
	push	bp
	mov		bp,sp
	cmp		word[bp+4], 0
	je		Restore
	pushf
	push    cs
	;push	ip
	push 	ax
	push 	bx
	push 	cx
	push 	dx
	push 	si
	push 	di
	push 	ds
	push 	es
	mov		[YKCurrentTask+2], sp
	
	
Restore:	
	mov     sp, [bp+4]
	pop 	es
	pop 	ds
	pop 	di
	pop 	si
	pop 	dx
	pop 	cx
	pop 	bx
	pop 	ax
	mov		sp, bp
	pop		bp
	iret
