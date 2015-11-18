#ifndef YAKU_H
#define YAKU_H

#define MAX_TASKS 10
#define YKIDLE_STACKSIZE 256
#define YKIDLE_PRIORITY 100
#define MAX_SEM	10
#define MAX_QUEUES 10
#define MAX_EVENTS 10

void userTickHandler();
void myreset(void);
void mytick(void);
void mykeybrd(void);

#endif
