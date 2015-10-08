
#include "clib.h"
#include "yakk.h"
#include "yaku.h"

TCBptr YKRdyList;		/* a list of TCBs of all ready tasks
				   in order of decreasing priority */ 
TCBptr YKSuspList;		/* tasks delayed or suspended */
TCBptr YKAvailTCBList;		/* a list of available TCBs */
int YKTaskListSize;
char YKTasksRunning;
char YKNextingCount;
				  /* (extra one is for the idle task) */
TCB YK_TCB_Array[MAX_TASKS+1];	/* array to allocate all needed TCBs int YKIdleStk[YKIDLE_STACKSIZE]; //Allocate space for YK idle task*/

int YKIdleStk[YKIDLE_STACKSIZE]; //Do I need a +1 here

 //This is syntax for function pointers
 //void (*p1_foo)() = foo;
 //void (*p3_foo)() = &foo;


/*
 *
 */
void YKInitialize()
{
	//Initial tasklist size is 0
	YKTaskListSize = 0;

	//Create new task for Idle Task
	YKNewTask(YKIdleTask, (void *)&YKIdleStk[YKIDLE_STACKSIZE], YKIDLE_PRIORITY);
	YKRdyList = &YK_TCB_Array[0];
	
}

/*
 *
 */
void YKEnterISR()
{

}

/*
 *
 */
void YKExitISR()
{
	
}

/*
 *
 */
void YKIdleTask()
{

}

/*
 *
 */
void YKNewTask(void* functionPtr, void* stackPtr, int newTaskPriority)
{
	//Make a temporary TCB pointer
	TCBptr tmp_TCBptr;
	TCBptr tmp_next;
	TCBptr tmp_prev;

	//Check to see if we are creating the first TCB
	if(YKTaskListSize == 0)
	{
		//If first start available list
		YKAvailTCBList = (TCBptr) &YK_TCB_Array[YKTaskListSize];
		tmp_TCBptr = YKAvailTCBList;
	}
	else
	{
		//Initialize temp TCB pointer
		tmp_TCBptr = (TCBptr) &YK_TCB_Array[YKTaskListSize-1];
		
	}	
	//Set the TCBs initial values
	tmp_TCBptr->pc = functionPtr;
	tmp_TCBptr->bp = stackPtr;
	tmp_TCBptr->sp = stackPtr;
	tmp_TCBptr->ready = 1;
	tmp_TCBptr->blocked = 0;
	tmp_TCBptr->delay = 0;
	//tmp_TCBptr->stackBeginAddress = 0;
	//tmp_TCBptr->stackEndAddress = 0;
	tmp_TCBptr->priority = newTaskPriority;

	//Initialize to beginning of Ready list
	tmp_next = YKRdyList;
	tmp_prev = NULL;

	//Go through the loop until we reach the end
	do {
		if(tmp_TCBptr){
			
		}
	} while(tmp_next != NULL);
	
	
	YKTaskListSize++;	
}

/*
 *
 */
void YKRun()
{

}

/*
 *
 */
void YKScheduler()
{

}

/*
 *
 */
void YKDispatcher()
{

}


/*
 *
 */
void YKCtxSwCount()
{

}

/*
 *
 */
void YKIdleCount()
{

}
