
#include "clib.h"

extern int KeyBuffer;
int tickNumber;

void resetHandler(){
	exit(0);
}

void tickHandler(){
    printNewLine();
    printString("TICK  ");
	printInt(tickNumber);
    printNewLine();
	tickNumber++;
}



void keyboardHandler(){
	int i;

	if(KeyBuffer == 'd'){
		printNewLine();
		printString("DELAY KEY PRESSED");       // Print string
		i = 0;
		while(i < 6000){
			i++;
		}
		printString("DELAY COMPLETE");       // Print string
		printNewLine();
	}

	printNewLine();
	printString("KEYPRESS ");       // Print string
    printChar(KeyBuffer);
	printString(" IGNORED");       // Print string
    printNewLine();

}
