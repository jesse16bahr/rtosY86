

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

	;cmp		word[YKNextTask+14],0
	;jne		saveContext
	;mov		word[YKNextTask+14],1
	;mov		bx, word[YKNextTask]
	;mov		sp, word[bx+4]
	;call	word[bx]

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
	cmp		word[YKNextTask+14],0
	je		Restore

savePointers:
	mov		bx,word[YKCurrentTask]
	mov		word[bx+4], sp
	mov		word[bx+2], bp
	
Restore:	
	mov		word[YKNextTask+14],1  ;stop from doing first time stuff
	mov		bx, word[YKNextTask]	
	mov		bp, word[bx+4]			;Grab base pointer
	mov		cx, bx
	mov		word[bp+2], bx
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
