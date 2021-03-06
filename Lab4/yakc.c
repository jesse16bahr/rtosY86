
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
unsigned short YKSemListSize;
unsigned short YKCtxSwCount;
char YKTasksRunning;
char YKInterruptDepth;
unsigned short YKIdleCount;
				  /* (extra one is for the idle task) */
TCB YK_TCB_Array[MAX_TASKS+1];	/* array to allocate all needed TCBs int YKIdleStk[YKIDLE_STACKSIZE]; //Allocate space for YK idle task*/

YKQ YKQ_Array[MAX_QUEUES];
YKEVENT YKEVENT_Array[MAX_EVENTS];
unsigned int YKNumberOfEvents;

YKSEM YKSEM_Array[MAX_SEM]; /* array to allocate space for semaphores */
unsigned int YKNumberOfQueues;

int YKIdleStk[YKIDLE_STACKSIZE]; //Do I need a +1 here


/*
 *
 */
YKEVENT *YKEventCreate(unsigned short initialValue)
{

	YKEVENT *newEvent = (YKEVENT *) &YKEVENT_Array[YKNumberOfEvents];
	YKNumberOfEvents++;
	newEvent->flags = initialValue;
	newEvent->pendListStart = NULL;
	return newEvent;
}

/*
 * Pend if the event(s) have not happened yet
 */
unsigned short YKEventPend(YKEVENT *event, unsigned eventMask, int waitMode)
{
	TCBptr tmp_current;
	TCBptr tmp_next;
	TCBptr tmp_prev;

	unsigned char pend = TRUE;	
	if(waitMode == EVENT_WAIT_ANY)
	{
		if(event->flags & eventMask != 0)
		{
			pend = FALSE;
		}
	}
	else if(waitMode == EVENT_WAIT_ALL)
	{
		if(event->flags & eventMask == eventMask)
		{
			pend = FALSE;
		}
	}

	if(pend == TRUE)
	{
		
		YKEnterMutex();			//Start critical section
		YKCurrentTask->ready = FALSE;
		YKCurrentTask->waitCondition = waitMode;
		YKCurrentTask->waitValue = eventMask;
		tmp_current = event->pendListStart;

		if(tmp_current != NULL)
		{
			//Go through the loop until we reach the end
			do 
			{
				if(YKCurrentTask->priority < tmp_current->priority)
				{
					YKCurrentTask->EventNext = tmp_current;
					YKCurrentTask->EventPrev = tmp_current->EventPrev;
					tmp_current->EventPrev = YKCurrentTask;
					if(YKCurrentTask->EventPrev == NULL)
					{
						event->pendListStart = YKCurrentTask;
					}
					else
					{
						tmp_prev = YKCurrentTask->EventPrev; //This is for in between
						tmp_prev->EventNext = YKCurrentTask;
					}
					tmp_current = NULL;
				}
				else if(tmp_current->EventNext == NULL){
					tmp_current->EventNext = YKCurrentTask;
					YKCurrentTask->EventPrev = tmp_current;
					YKCurrentTask->EventNext = NULL;
				}

				if(tmp_current != NULL)
				{
						tmp_next = tmp_current->EventNext;
						tmp_current = tmp_next;
						tmp_current = NULL;
				}
	
			} while(tmp_current != NULL);
		}
		else
		{
			event->pendListStart = YKCurrentTask;
			YKCurrentTask->EventPrev = NULL;
		}
		YKExitMutex();		//End critical section

		YKScheduler();
	}	

	return event->flags;	
}

/*
 *
 */
void YKEventSet(YKEVENT *event, unsigned eventMask)
{

	short unleashedtasks = 0;
	TCBptr tmp_current = event->pendListStart;
	TCBptr temp_ptr;
	YKEnterMutex();
	event->flags = event->flags | eventMask;
	while(tmp_current != NULL)
	{
		// check stuff
		if(tmp_current->waitCondition == EVENT_WAIT_ANY)
		{
			if(tmp_current->waitValue & event->flags)
			{
				// wait any conditions met
				tmp_current->ready = TRUE;
				// remove from list.
				temp_ptr = tmp_current; // holds the pointer we are on currently
				tmp_current = tmp_current->EventNext; // moves to next pointer
				if(tmp_current != NULL)
				{
					tmp_current->EventPrev = temp_ptr->EventPrev; // update next pointer
					if(tmp_current->EventPrev != NULL)
					{
						tmp_current->EventPrev->EventNext = tmp_current; // update next pointer
					}
					temp_ptr = NULL;
				}
				unleashedtasks+=1;
			}else
			{
			// move along the list of pending things
			tmp_current = tmp_current->EventNext; // moves to next pointer
			}
		}else
		if(tmp_current->waitCondition == EVENT_WAIT_ALL)
		{
			if(tmp_current->waitValue&event->flags == tmp_current->waitValue)
			{
				// wait all conditions met
				tmp_current->ready = TRUE;
				// remove from list.
				temp_ptr = tmp_current; // holds the pointer we are on currently
				tmp_current = tmp_current->EventNext; // moves to next pointer
				if(tmp_current != NULL)
				{
					tmp_current->EventPrev = temp_ptr->EventPrev; // update next pointer
					if(tmp_current->EventPrev != NULL)
					{
						tmp_current->EventPrev->EventNext = tmp_current; // update next pointer
					}					
					temp_ptr = NULL;
				}
				unleashedtasks+=1;
			}else
			{
				tmp_current = tmp_current->EventNext; // moves to next pointer	
			}
		}else
		{
		// move along the list of pending things
		tmp_current = tmp_current->EventNext; // moves to next pointer	
		}
	}
	// All tasks that should have been made ready are now ready.
	if(YKInterruptDepth == 0 && unleashedtasks > 0)
	{
		YKExitMutex();
		YKScheduler();	
	}
	else
	{
		YKExitMutex();
	}
	
	
}

/*
 *
 */
void YKEventReset(YKEVENT *event, unsigned eventMask)
{
	//clear the gits that are set in the mask
	event->flags &= ~eventMask;
}



/*
 *
 */
YKQ *YKQCreate(void** start, unsigned int size)
{
	YKQ *newQueue = (YKQ *) &YKQ_Array[YKNumberOfQueues];
    YKNumberOfQueues++;

	//Assign our never changing
	newQueue->baseAddress = start;
	newQueue->endAddress = start+size;
    newQueue->size = size;

	newQueue->length = 0;
	newQueue->head = start;
	newQueue->tail = start;

	return newQueue;
}

/*
 *
 */
void *YKQPend(YKQ* queue)
{
	void** tempHead;
	TCBptr tmp_current;
	TCBptr tmp_next;
	TCBptr tmp_prev;

	YKEnterMutex();

	//If we have nothing to grab in our queue
	if(queue->length == 0)
	{
		YKCurrentTask->ready = FALSE;

		tmp_current = queue->pendListStart;
		if(tmp_current != NULL)
		{
			//Go through the loop until we reach the end
			do 
			{
				if(YKCurrentTask->priority < tmp_current->priority)
				{
					YKCurrentTask->QueNext = tmp_current;
					YKCurrentTask->QuePrev = tmp_current->QuePrev;
					tmp_current->QuePrev = YKCurrentTask;
					if(YKCurrentTask->QuePrev == NULL)
					{
						queue->pendListStart = YKCurrentTask;
					}
					else
					{
						tmp_prev = YKCurrentTask->QuePrev; //This is for in between
						tmp_prev->QueNext = YKCurrentTask;
					}

					tmp_current = NULL;
				}
				else if(tmp_current->QueNext == NULL){
					tmp_current->QueNext = YKCurrentTask;
					YKCurrentTask->QuePrev = tmp_current;
					YKCurrentTask->QueNext = NULL;
				}

				if(tmp_current != NULL)
				{
					tmp_next = tmp_current->QueNext;
					tmp_current = tmp_next;
				}
	
			} while(tmp_current != NULL);
		}
		else
		{
			queue->pendListStart = YKCurrentTask;
			YKCurrentTask->QuePrev = NULL;
		}
		
		YKScheduler();
	}	

	YKExitMutex();

	YKEnterMutex();
	//If we do have something to grab in our queue
	
	tempHead = queue->head;
	if(queue->length > 0)
	{
			queue->head++;
			if(queue->head >= queue->endAddress)
			{
				queue->head = queue->baseAddress;
			}
			queue->length--;

		return *tempHead;
	}

	YKExitMutex();

	
}

/*
 *
 */
int YKQPost(YKQ* queue, void* message)
{

	TCBptr tmp_current = queue->pendListStart;

	 

	if(queue->length < queue->size)
	{
		YKEnterMutex();
		
		//Always post to the queue if it is not full
		*queue->tail = message;
		queue->tail++;
		queue->length++;
		if(queue->tail >= queue->endAddress)
		{
			queue->tail = queue->baseAddress;
		}

		//Only make a task ready if there is one waiting
		if(tmp_current != NULL)
		{
			//Set the first task pending back to enabled
			tmp_current->ready = TRUE;				//Allow the current block to run
			queue->pendListStart = tmp_current->QueNext;		//make list start at the next item now
			queue->pendListStart->QuePrev = NULL;		//make list start at the next item now
			tmp_current->QueNext = NULL;				//take off the front
			tmp_current->QuePrev = NULL;				//	"   "
	
		}

		if(YKInterruptDepth == 0)
		{
			YKExitMutex();
			YKScheduler();	
		}

		YKExitMutex();

		return 1;
	}
	else
	{
		return 0;
	}
}


/*
 *
 */
void YKInitialize()
{
	YKInterruptDepth = 0;
	YKSemListSize = 0;
	YKIdleCount = 0;

	//Initial tasklist size is 0
	YKTaskListSize = 0;
	YKNumberOfQueues = 0;
	YKNumberOfEvents = 0;
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
	newTCB->sem_block = FALSE;	
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
 * Create a new semaphore by pointing to an available position in semaphore array
 */
YKSEM* YKSemCreate(signed short initSemVal)
{
		YKSEM* newSem = (YKSEM*)&YKSEM_Array[YKSemListSize];
		newSem->value = initSemVal;
		YKSemListSize++;
		return newSem;
}

/*
 *
 */
void YKSemPost(YKSEM* sem)
{
	TCBptr tmp_current = sem->pendListStart;
	YKEnterMutex();
	
	if(tmp_current != NULL && sem->value == 0)
	{
		//Set the first task pending back to enabled
		tmp_current->sem_block = FALSE;				//Allow the current block to run
		sem->pendListStart = tmp_current->SemNext;		//make list start at the next item now
		sem->pendListStart->SemPrev = NULL;		//make list start at the next item now
		tmp_current->SemNext = NULL;				//take off the front
		tmp_current->SemPrev = NULL;				//	"   "
	
	}
	else
	{
		sem->value++;
	}

	if(YKInterruptDepth == 0)
	{
		YKExitMutex();
		YKScheduler();	
	}
	else
	{
		YKExitMutex();
	}
}

/*
 *
 */
void YKSemPend(YKSEM* sem)
{
	TCBptr tmp_current;
	TCBptr tmp_next;
	TCBptr tmp_prev;
	 
	YKEnterMutex();
	//printInt(sem);
	//printNewLine();
	//printInt(sem->value);
	if(sem->value > 0)
	{
		//printString("TAKE SEM\r\n");
		//printNewLine();
		sem->value--;
		YKCurrentTask->sem_block = FALSE;
		YKExitMutex();
		return;
	}
	else
	{
		//printString("PEND\r\n");
		//printNewLine();
		tmp_current = sem->pendListStart;
		if(tmp_current != NULL)
		{
			//Go through the loop until we reach the end
			do 
			{
				if(YKCurrentTask->priority < tmp_current->priority)
				{
					YKCurrentTask->SemNext = tmp_current;
					YKCurrentTask->SemPrev = tmp_current->SemPrev;
					tmp_current->SemPrev = YKCurrentTask;
					if(YKCurrentTask->SemPrev == NULL)
					{
						sem->pendListStart = YKCurrentTask;
					}
					else
					{
						tmp_prev = YKCurrentTask->SemPrev; //This is for in between
						tmp_prev->SemNext = YKCurrentTask;
					}

					tmp_current = NULL;
				}
				else if(tmp_current->SemNext == NULL){
					tmp_current->SemNext = YKCurrentTask;
					YKCurrentTask->SemPrev = tmp_current;
					YKCurrentTask->SemNext = NULL;
				}

				if(tmp_current != NULL)
				{
					tmp_next = tmp_current->SemNext;
					tmp_current = tmp_next;
				}
	
			} while(tmp_current != NULL);
		}
		else
		{
			sem->pendListStart = YKCurrentTask;
			YKCurrentTask->SemPrev = NULL;
		}
		
		YKCurrentTask->sem_block = TRUE;	
		YKExitMutex();
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
		if(check_Ptr->ready == TRUE && check_Ptr->delay == 0 && check_Ptr->sem_block == FALSE)
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
