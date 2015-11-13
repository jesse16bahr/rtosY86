
#include "clib.h"
#include "yakk.h"
#include "yaku.h"

extern int KeyBuffer;
extern YKSEM *NSemPtr;
int YKTickNumber = 0;

void resetHandler(){
	myreset();
}

void YKTickHandler(){
	// This pointer will update as we move through the task list.
	TCBptr check_Ptr = YKRdyList;
	YKTickNumber++;
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
		i = 0;
		while(i < 6000){
			i++;
		}
	}
	if(KeyBuffer == 'p'){
		YKSemPost(NSemPtr);
	}
	else
	{
		mykeybrd();
	}

}

