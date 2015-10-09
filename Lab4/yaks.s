

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

	cmp		word[YKNextTask+14],0
	jne		saveContext
	mov		word[YKNextTask+14],1
	mov		bx, word[YKNextTask]
	call	word[bx]

saveContext:
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
	mov		word[YKCurrentTask+4], sp
	
	
Restore:	
	mov     sp, word[YKNextTask+4]
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
