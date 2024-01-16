#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "pqueue.h"

// In data_structures.h




void dispatcher(char *type, Data *processes, const int size);
void schedule_FIFO(pQueue *q);
void schedule_SJF(pQueue *q);
void schedule_RR(pQueue *q, int period);

#endif /*SCHEDULER_H_*/
