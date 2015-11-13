
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

	unsigned short sem_block;   //To help us know if the task is is blocked by a semaphore
	unsigned short ready;      //To help us know if task is ready to run
	unsigned short hasRun;

	TCBptr prev;
	TCBptr next;

	TCBptr SemPrev;
	TCBptr SemNext;

	TCBptr QuePrev;
	TCBptr QueNext;

} TCB;

typedef struct queue
{
	void** baseAddress;
	void** endAddress;
	unsigned int size;

	signed int length;
	void** head;
	void** tail;  

	TCBptr pendListStart;

} YKQ;


extern TCB  YK_TCB_Array[MAX_TASKS+1];	/* array to allocate all needed TCBs*/
extern unsigned short YKCtxSwCount;
extern unsigned short YKIdleCount;
extern TCBptr YKCurrentTask;
extern TCBptr YKRdyList;
extern TCBptr YKNextTask;
extern YKTickNum;

typedef struct semaphore *YKSEMptr; 
typedef struct semaphore
{
	signed short value;
	TCBptr pendListStart;
} YKSEM;

extern YKSEM YKSEM_Array[MAX_SEM];

/*
 *
 */
YKQ *YKQCreate(void** start, unsigned int size);

/*
 *
 */
void *YKQPend(YKQ* queue);

/*
 *
 */
int YKQPost(YKQ* queue, void* message);

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
YKSEM* YKSemCreate(signed short initSemVal);

/*
 *
 */
void YKSemPost(YKSEM* sem);

/*
 *
 */
void YKSemPend(YKSEM* sem);

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
