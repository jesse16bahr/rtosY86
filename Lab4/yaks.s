YKEnterMutex:
	push bp
	mov  bp, sp
	cli
	mov		sp, bp
	pop bp
	ret

YKExitMutex:
	push bp
	mov  bp, sp
	sti
	mov		sp, bp
	pop bp
	ret

YKDispatcher2:
	pushf
	push    cs
	push	word[bp+2]						;We need this because flags and cs are pushed first
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

	mov		bx, word[YKNextTask]			;
	mov		word[YKCurrentTask], bx			;YKCurrentTask = YKNextTask			
	mov		bp, word[bx+2]					;bp = YKNextTask->bp
	mov		sp, word[bx+4]					;sp = YKNextTask->sp
	mov		cx, word[bx]					;cx = YKNextTask->pc
	sti
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
	mov		bx, word[YKCurrentTask]			;Grab address of current Tasks TCB
	mov		word[bx+2], bp
	mov		word[bx+4], sp
	
	
Restore:									;This is where we switch tasks
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


