
#include "clib.h"
#include "yakk.h"
#include "yaku.h"

extern int KeyBuffer;
int YKTickNumber = 0;

void resetHandler(){
	exit(0);
}

void YKTickHandler(){
	// This pointer will update as we move through the task list.
	TCBptr check_Ptr = YKRdyList;
	YKTickNumber++;
    printNewLine();
    printString("TICK  ");
	printInt(YKTickNumber);
    printNewLine();
	

	// Follow the linked list, if tasks delay is greater than 0 decrement the delay
	// Tasks should be stored in order of priority.
	while(check_Ptr != NULL)
	{
		if(check_Ptr->delay > 0)
		{
			check_Ptr->delay--;
		}
		
		check_Ptr = check_Ptr->next;
	}
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
	else
	{
		printNewLine();
		printString("KEYPRESS ");       // Print string
		printChar(KeyBuffer);
		printString(" IGNORED");       // Print string
		printNewLine();
	}

}
