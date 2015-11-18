#####################################################################
# ECEn 425 Lab 4a Makefile

lab7.bin:	lab7final.s
		nasm lab7final.s -o lab7.bin -l lab7.lst

lab7final.s:	clib.s myisr.s myinth.s yaks.s lab7app.s yakc.s lab7inth.s
		cat  clib.s myisr.s myinth.s yaks.s lab7app.s yakc.s lab7inth.s > lab7final.s

lab7app.s:	lab7app.c
		cpp lab7app.c lab7app.i
		c86 -g lab7app.i lab7app.s

myinth.s:	myinth.c
		cpp myinth.c myinth.i
		c86 -g myinth.i myinth.s

yakc.s:		yakc.c
		cpp yakc.c yakc.i
		c86 -g yakc.i yakc.s

lab7inth.s:     lab7inth.c
		cpp lab7inth.c lab7inth.i
		c86 -g lab7inth.i lab7inth.s

clean:
		rm lab7inth.i lab7inth.s lab7.bin lab7.lst lab7final.s myinth.s myinth.i yakc.s yakc.i lab7app.s lab7app.i yakc.c~ yakk.h~ yaks.s~ yaku.h~

