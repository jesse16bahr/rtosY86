#####################################################################
# ECEn 425 Lab 4a Makefile

lab6.bin:	lab6final.s
		nasm lab6final.s -o lab6.bin -l lab6.lst

lab6final.s:	clib.s myisr.s myinth.s yaks.s lab6app.s yakc.s lab6inth.s
		cat lab6inth.s clib.s myisr.s myinth.s yaks.s lab6app.s yakc.s > lab6final.s

lab6app.s:	lab6app.c
		cpp lab6app.c lab6app.i
		c86 -g lab6app.i lab6app.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

yakc.s:		yakc.c
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

lab6inth.s:     lab6inth.c
		cpp lab6inth.c lab6inth.i
		c86 -g lab6inth.i lab6inth.s

clean:
		rm lab6inth.i lab6inth.s lab6.bin lab6.lst lab6final.s myinth.s myinth.i yakc.s yakc.i lab6app.s lab6app.i yakc.c~ yakk.h~ yaks.s~ yaku.h~

