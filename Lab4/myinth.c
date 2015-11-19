#include "clib.h"
#include "yakk.h"
#include "yaku.h"
#include "lab7defs.h"

extern int KeyBuffer;
extern YKSEM *NSemPtr;
int YKTickNum = 0;

void resetHandler(){
	exit(0);
}

void YKTickHandler(){
	// This pointer will update as we move through the task list.
	TCBptr check_Ptr = YKRdyList;
	YKTickNum++;
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



void keyboardHandler(void)
{
    char c;
    c = KeyBuffer;

    if(c == 'a') YKEventSet(charEvent, EVENT_A_KEY);
    else if(c == 'b') YKEventSet(charEvent, EVENT_B_KEY);
    else if(c == 'c') YKEventSet(charEvent, EVENT_C_KEY);
    else if(c == 'd') YKEventSet(charEvent, EVENT_A_KEY | EVENT_B_KEY | EVENT_C_KEY);
    else if(c == '1') YKEventSet(numEvent, EVENT_1_KEY);
    else if(c == '2') YKEventSet(numEvent, EVENT_2_KEY);
    else if(c == '3') YKEventSet(numEvent, EVENT_3_KEY);
    else {
        print("\nKEYPRESS (", 11);
        printChar(c);
        print(") IGNORED\n", 10);
    }
}
