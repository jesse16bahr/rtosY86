
#ifndef YAKK_H
#define YAKK_H

#include "yaku.h"

#define NULL 0
#define FALSE 0
#define TRUE !FALSE


typedef struct taskblock *TCBptr;
typedef struct taskblock
{
	void* pc; // Returns the proper instruction pointer to the code
	void* sp; //tasks stack pointer
	void* bp; //base pointer of tasks stack frame
	

	unsigned short priority;	 //Lower numbers have higher priority
	unsigned short delay;        //Help us know if task is delayed

	unsigned short blocked;   //To help us know if the task is is blocked
	unsigned short ready;      //To help us know if task is ready to run
	unsigned short hasRun;

	TCBptr prev;
	TCBptr next;

} TCB;

extern TCB  YK_TCB_Array[MAX_TASKS+1];	/* array to allocate all needed TCBs*/
extern unsigned short YKCtxSwCount;
extern unsigned short YKIdleCount;
extern TCBptr YKCurrentTask;
extern TCBptr YKRdyList;
extern TCBptr YKNextTask;

/*
 *
 */
void YKInitialize();

/*
 * Function Prototype for assembly function that turns off interrupts
 * Found in yaks.s
 */
void YKEnterMutex();

/*
 * Function prototype for assembly function that turns on interrupts
 * Found in yaks.s
 */
void YKExitMutex();

/*
 * Function prototype for assembly function that turns on interrupts
 * Found in yaks.s
 */
void YKIdleTask();


/*
 *
 */
void YKDelayTask(unsigned int count);

/*
 *
 */
void YKNewTask();

/*
 *
 */
void YKRun();

/*
 *
 */
void YKScheduler();

/*
 *
 */
void YKDispatcher();

/*
 *
 */
void YKDispatcher2();

#endif
