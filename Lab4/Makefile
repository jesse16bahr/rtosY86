#####################################################################
# ECEn 425 Lab 4a Makefile

lab4c.bin:	lab4cfinal.s
		nasm lab4cfinal.s -o lab4c.bin -l lab4c.lst

lab4cfinal.s:	clib.s myisr.s myinth.s yaks.s lab4c_app.s yakc.s
		cat clib.s myisr.s myinth.s yaks.s lab4c_app.s yakc.s > lab4cfinal.s

lab4c_app.s:	lab4c_app.c
		cpp lab4c_app.c lab4c_app.i
		c86 -g lab4c_app.i lab4c_app.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

yakc.s:		yakc.c
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

clean:
		rm lab4c.bin lab4c.lst lab4cfinal.s myinth.s myinth.i yakc.s yakc.i lab4c_app.s lab4c_app.i yakc.c~ yakk.h~ yaks.s~ yaku.h~

