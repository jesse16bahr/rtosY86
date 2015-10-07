
#include "clib.h"
#include "yakk.h"

TCB all_tcbs[MAX_TASKS_NUM];

TCBptr YKRdyList;		/* a list of TCBs of all ready tasks
				   in order of decreasing priority */ 
TCBptr YKSuspList;		/* tasks delayed or suspended */
TCBptr YKAvailTCBList;		/* a list of available TCBs */
				  /* (extra one is for the idle task) */
TCB YKTCBArray[MAX_TASKS+1];	/* array to allocate all needed TCBs int YKIdleStk[YKIDLE_STACKSIZE]; //Allocate space for YK idle task

/*
 *
 */
void YKInitialize(){
	
	YKNewTask(YKIdleTask, (void *)&YKIdleStk, YKIDLE_PRIORITY));	
}

/*
 *
 */
void YKEnterMutex(){
	
}

/*
 *
 */
void YKExitMutex(){

}

/*
 *
 */
void YKIdleTask(){

}

/*
 *
 */
void YKNewTask(){
	
}

/*
 *
 */
void YKRun(){

}

/*
 *
 */
void YKScheduler(){

}

/*
 *
 */
void YKDispatcher(){

}

}

/*
 *
 */
void YKCtxSwCount(){

}

/*
 *
 */
void YKIdleCount(){

}
