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

YKDispatcher2:
	push	bp
	mov		bp,sp
	pushf
	push    cs
	push	word[bp+2]

	mov		bx, word[YKNextTask]			;
	mov		word[YKCurrentTask], bx			;YKCurrentTask = YKNextTask			
	mov		bp, word[bx+2]					;bp = YKNextTask->bp
	mov		sp, word[bx+4]					;sp = YKNextTask->sp
	mov		cx, word[bx]					;cx = YKNextTask->pc
	pushf
	push    cs
	push	cx								;push the current ip
	iret

YKDispatcher:
	pushf									;save the context first
	push    cs
	push	word[bp+2]
	push	bp
	mov		bp,sp
	push 	ax
	push 	bx
	push 	cx
	push 	dx
	push 	si
	push 	di
	push 	ds
	push 	es
	mov		bx, word[YKCurrentTask]
	mov		word[bx+2], bp
	mov		word[bx+4], sp
	
	
Restore:	
	mov		bx, word[YKNextTask]			;YKCurrentTask = YKNextTask
	mov		word[YKCurrentTask], bx			; 		"			"			
	mov		bp, word[bx+2]					;bp = YKNextTask->bp
	mov		sp, word[bx+4]					;sp = YKNextTask->sp
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


