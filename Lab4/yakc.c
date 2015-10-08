
#include "clib.h"
#include "yakk.h"
#include "yaku.h"

TCBptr YKRdyList;		/* a list of TCBs of all ready tasks
				   in order of decreasing priority */ 
TCBptr YKSuspList;		/* tasks delayed or suspended */
TCBptr YKAvailTCBList;		/* a list of available TCBs */
unsigned short YKTaskListSize;
unsigned short YKCtxSwCount;
char YKTasksRunning;
char YKInterruptDepth;

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
	YKInterruptDepth = 0;

	//Initial tasklist size is 0
	YKTaskListSize = 0;

	//Create new task for Idle Task
	YKRdyList = NULL;
	YKNewTask(YKIdleTask, (void *)&YKIdleStk[YKIDLE_STACKSIZE], YKIDLE_PRIORITY);
	
}

/*
 *
 */
void YKEnterISR()
{
	YKInterruptDepth++;
}

/*
 *
 */
void YKExitISR()
{
	if(YKInterruptDepth > 0)
	{
		YKInterruptDepth--;
	}
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
	TCBptr newTCB;
	TCBptr tmp_next;
	TCBptr tmp_prev;
	TCBptr tmp_current;
	//Check to see if we are creating the first TCB
	if(YKTaskListSize == 0)
	{
		//If first start available list
		YKAvailTCBList = (TCBptr) &YK_TCB_Array[YKTaskListSize];
		newTCB = YKAvailTCBList;
	}
	else
	{
		//Initialize temp TCB pointer
		newTCB = (TCBptr) &YK_TCB_Array[YKTaskListSize-1];
		
	}	

	//Set the TCBs initial values
	newTCB->pc = functionPtr;
	newTCB->bp = stackPtr;
	newTCB->sp = stackPtr;
	newTCB->ready = 1;
	newTCB->blocked = 0;
	newTCB->delay = 0;
	newTCB->priority = newTaskPriority;

	//Initialize to beginning of Ready list
	
	tmp_current = YKRdyList;
	tmp_next = YKRdyList->next;
	tmp_prev = YKRdyList->prev;

	if(tmp_current != NULL)
	{
		//Go through the loop until we reach the end
		do 
		{
			if(newTCB->priority < tmp_current->priority)
			{
				newTCB->next = tmp_current;
				newTCB->prev = tmp_current->prev;
				tmp_current->prev = newTCB;
				if(newTCB->prev == NULL){
					YKRdyList = newTCB;
				}
				tmp_next = tmp_current->next;
				tmp_current  = tmp_current->next;
				tmp_current = NULL;
			}
			else if(tmp_current->next == NULL){
				tmp_current->next = newTCB;
				newTCB->prev =
			}

			
		} while(tmp_next != NULL);
	}
	else
	{
		YKRdyList = newTCB;
	}
	
	
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
	// This pointer will update as we move through the task list.
	TCBptr check_Ptr = YKRdyList;
	// Follow the linked list, check task to see if it is ready, if is ready call the Dispatcher
	// Tasks should be stored in order of priority.
	while(check_Ptr != NULL)
	{
		if(check_Ptr->ready == TRUE)
		{
			// This task is the highest ready task, so call scheduler.
			YKDispatcher(check_Ptr);
			// Set pointer to null so loop is over.
		}else
		{
			// We have not found an available task yet, so go to next.
			check_Ptr = check_Ptr->next;
		}
	}
}

/*
 *
 */
void YKDispatcher(TCBptr dispatch_Task)
{

}
