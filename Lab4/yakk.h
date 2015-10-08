
#ifndef YAKK_H
#define YAKK_H

#include "yaku.h"

#define NULL 0

typedef struct taskblock *TCBptr;
typedef struct taskblock
{
	void* sp; //tasks stack pointer
	void* bp; //base pointer of tasks stack frame
	void* pc; // Returns the proper instruction pointer to the code

	unsigned short priority;	 //Lower numbers have higher priority
	unsigned short delay;        //Help us know if task is delayed

	unsigned char blocked;   //To help us know if the task is is blocked
	unsigned char ready;      //To help us know if task is ready to run

	TCBptr prev;
	TCBptr next;

} TCB;

extern TCB  YK_TCB_Array[MAX_TASKS+1];	/* array to allocate all needed TCBs


/*
 *
 */
void YKInitialize();

/*
 *
 */
void YKEnterMutex();

/*
 * Function Prototype for assembly function that turns off interrupts
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
void YKCtxSwCount();

/*
 *
 */
void YKIdleCount();

#endif
