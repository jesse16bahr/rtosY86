
#include "clib.h"
#include "yakk.h"
#include "yaku.h"

extern int KeyBuffer;
extern YKSEM *NSemPtr;
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
	if(KeyBuffer == 'p'){
		YKSemPost(NSemPtr);
		/*printNewLine();
		printString("KEYPRESS ");       // Print string
		printChar(KeyBuffer);
		printString(" IGNORED");       // Print string
		printNewLine();*/
	}
	else
	{
		globalFlag = TRUE;
	}

}



/* 
File: lab6inth.c
Revision date: 4 November 2009
Description: Sample interrupt handler code for EE 425 lab 6 (Message queues)
*/

/*
#include "lab6defs.h"
#include "yakk.h"
#include "clib.h"

extern YKQ *MsgQPtr; 
extern struct msg MsgArray[];
extern int GlobalFlag;

void myreset(void)
{
    exit(0);
}
*/
/*
void mytick(void)
{
    static int next = 0;
    static int data = 0;
*/
    /* create a message with tick (sequence #) and pseudo-random data */
/*
    MsgArray[next].tick = YKTickNum;
    data = (data + 89) % 100;
    MsgArray[next].data = data;
    if (YKQPost(MsgQPtr, (void *) &(MsgArray[next])) == 0)
	printString("  TickISR: queue overflow! \n");
    else if (++next >= MSGARRAYSIZE)
	next = 0;
}	       
*/
/*
void mykeybrd(void)
{
    GlobalFlag = 1;
}
*/

