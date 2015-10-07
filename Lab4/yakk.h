
#ifndef YAKK_H
#define YAKK_H

#include "yaku.h"

typedef struct taskblock *TCBptr;
typedef struct taskblock
{
unsigned short stackBeginAddress; //define a separate task for this task
unsigned short stackEndAddress;   //define a separate task for this task

void* sp; //tasks stack pointer
void* bp; //base pointer of tasks stack frame
void* pc; // Returns the proper instruction in the code

unsigned short priority;	 //Lower numbers have higher priority
unsigned short delay;        //Help us know if task is delayed

unsigned char blockedFlag;   //To help us know if the task is is blocked
unsigned char readyFlag;      //To help us know if task is ready to run

TCBptr prev;
TCBptr next;

} TCB;

extern TCB    YKTCBArray[MAX_TASKS+1];	/* array to allocate all needed TCBs




/*
 *
 */
void YKInitialize();

/*
 *
 */
void YKEnterMutex();

/*
 *
 */
void YKExitMutex();

/*
 *
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
