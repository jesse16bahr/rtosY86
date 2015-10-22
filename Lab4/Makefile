#####################################################################
# ECEn 425 Lab 4a Makefile

lab4d.bin:	lab4dfinal.s
		nasm lab4dfinal.s -o lab4d.bin -l lab4d.lst

lab4dfinal.s:	clib.s myisr.s myinth.s yaks.s lab4d_app.s yakc.s
		cat clib.s myisr.s myinth.s yaks.s lab4d_app.s yakc.s > lab4dfinal.s

lab4d_app.s:	lab4d_app.c
		cpp lab4d_app.c lab4d_app.i
		c86 -g lab4d_app.i lab4d_app.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

yakc.s:		yakc.c
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

clean:
		rm lab4d.bin lab4d.lst lab4dfinal.s myinth.s myinth.i yakc.s yakc.i lab4d_app.s lab4d_app.i yakc.c~ yakk.h~ yaks.s~ yaku.h~

