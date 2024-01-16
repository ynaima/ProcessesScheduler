#ifndef PQUEUE_H_
#define PQUEUE_H_

# include "data.h"

# define True 1
# define False 0

typedef struct {
	Data *array; 	//array containing data items
	int front;		//index of first item in Queue
	int rear;		//index of last item in Queue
	int capacity;	//maximum queue length
	char priority;  //'H' = Higher priority first
					//'L' = Lower priority first
} pQueue;

pQueue* create_pqueue(int capacity, char priority);
int is_empty_pqueue(pQueue *q);
int len_pqueue(pQueue *q);
int is_full_pqueue(pQueue *q);
void destroy_pqueue(pQueue **q);
void print_pqueue(pQueue *q);
int get_capacity_pqueue(pQueue *q);

int insert_pqueue(pQueue *q, Data *d);
Data* peek_pqueue(pQueue *q);
Data* remove_pqueue(pQueue *q);

int insert_pqueue2(pQueue *q, Data *d);
Data* peek_pqueue2(pQueue *q);
Data* remove_pqueue2(pQueue *q);

#endif /* PQUEUE_H_ */
