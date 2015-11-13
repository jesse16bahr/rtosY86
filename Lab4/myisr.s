

Reset:
	sti			;turn on interrupts
	call resetHandler	;call interrupt handler
	cli			;turn off interrupts
	iret			;return from interrupts


Tick:
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
	call	YKEnterISR
	sti
	call    YKTickHandler
	call    mytick 
	cli
	mov		al, 0x20	; Load nonspecific EOI value (0x20) into register al
	out		0x20, al	; Write EOI to PIC (port 0x20)
	call    YKExitISR
	pop 	es
	pop 	ds
	pop 	di
	pop 	si
	pop 	dx
	pop 	cx
	pop 	bx
	pop 	ax
	;mov		sp, bp
	pop 	bp
	iret

Keyboard:
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
	call	YKEnterISR
	sti
	call keyboardHandler
	cli
	mov		al, 0x20	; Load nonspecific EOI value (0x20) into register al
	out		0x20, al	; Write EOI to PIC (port 0x20)
	call    YKExitISR
	pop 	es
	pop 	ds
	pop 	di
	pop 	si
	pop 	dx
	pop 	cx
	pop 	bx
	pop 	ax
	;mov		sp, bp
	pop 	bp
	iret
	

