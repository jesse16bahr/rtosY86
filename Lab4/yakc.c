
#include "clib.h"
#include "yakk.h"
#include "yaku.h"

TCBptr YKRdyList;		/* a list of TCBs of all ready tasks
				   in order of decreasing priority */ 
TCBptr YKSuspList;		/* tasks delayed or suspended */
TCBptr YKAvailTCBList;		/* a list of available TCBs */
TCBptr YKCurrentTask;
TCBptr YKNextTask;

unsigned short YKTaskListSize;
unsigned short YKCtxSwCount;
char YKTasksRunning;
char YKInterruptDepth;
unsigned short YKIdleCount;
				  /* (extra one is for the idle task) */
TCB YK_TCB_Array[MAX_TASKS+1];	/* array to allocate all needed TCBs int YKIdleStk[YKIDLE_STACKSIZE]; //Allocate space for YK idle task*/

int YKIdleStk[YKIDLE_STACKSIZE]; //Do I need a +1 here



/*
 *
 */
void YKInitialize()
{
	YKInterruptDepth = 0;
	YKIdleCount = 0;

	//Initial tasklist size is 0
	YKTaskListSize = 0;
	YKTasksRunning = FALSE;

	//Create new task for Idle Task
	YKRdyList = NULL;
	YKNewTask(YKIdleTask, (void *)&YKIdleStk[YKIDLE_STACKSIZE], YKIDLE_PRIORITY);
}

void YKDelayTask(unsigned int count)
{

	YKCurrentTask->delay = count;
	YKScheduler();
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
	
	//Call the Scheduler only if we are leaving all interrupts
	if(YKInterruptDepth == 0)
	{
		YKScheduler();
	}
}

/*
 *
 */
void YKIdleTask()
{
	unsigned char temp;
	while(TRUE)	
	{		
		temp++;
		temp++;
		YKIdleCount++;
	}
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
	unsigned int* temp_sp;

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
		newTCB = (TCBptr) &YK_TCB_Array[YKTaskListSize];
	}	
	temp_sp = stackPtr;

	temp_sp--;
	//*temp_sp = (unsigned int)functionPtr;
	//temp_sp--;
	//*temp_sp = (unsigned int)stackPtr;

	//Set the TCBs initial values
	newTCB->pc = functionPtr;
	newTCB->bp = temp_sp;
	newTCB->sp = temp_sp;
	newTCB->ready = TRUE;
	newTCB->blocked = FALSE;
	newTCB->delay = 0;
	newTCB->hasRun = FALSE;
	newTCB->priority = newTaskPriority;
	newTCB->next = NULL;
	newTCB->prev = NULL;

	

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
				if(newTCB->prev == NULL)
				{
					YKRdyList = newTCB;
				}
				else
				{
					tmp_prev = newTCB->prev;
					tmp_prev->next = newTCB;
				}

				tmp_current = NULL;
			}
			else if(tmp_current->next == NULL){
				tmp_current->next = newTCB;
				newTCB->prev = tmp_current;
				newTCB->next = NULL;
			}

			if(tmp_current != NULL)
			{
				tmp_next = tmp_current->next;
				tmp_current = tmp_next;
			}
	
		} while(tmp_current != NULL);
	}
	else
	{
		YKRdyList = newTCB;
	}

	YKTaskListSize++;						//Increment Task Size
	if(YKTasksRunning == TRUE){
		YKScheduler();
	}
	
	
}

/*
 *
 */
void YKRun()
{
	YKTasksRunning = TRUE;
	YKScheduler();
}

/*
 *
 */
void YKScheduler()
{
	
	// This pointer will update as we move through the task list.
	TCBptr check_Ptr; 

	YKEnterMutex();

	check_Ptr = YKRdyList;
	
	// Follow the linked list, check task to see if it is ready, if is ready call the Dispatcher
	// Tasks should be stored in order of priority.
	while(check_Ptr != NULL)
	{
		if(check_Ptr->ready == TRUE && check_Ptr->delay == 0)
		{
			
			YKNextTask = check_Ptr;

			if(YKNextTask->hasRun == 0) //Task hasn't run yet this is the first time
			{
				YKCtxSwCount++;
				YKNextTask->hasRun = 1;
				YKDispatcher2();
			}
			else if(YKNextTask == YKCurrentTask)
			{
				// This task is the highest ready task, so call scheduler.
				YKDispatcher();
			}
			else //Task has already run
			{
				YKCtxSwCount++;
				// This task is the highest ready task, so call scheduler.
				YKDispatcher();
			}
			
			check_Ptr = NULL;
		}
		
		check_Ptr = check_Ptr->next;
	}

	YKExitMutex();
}
