/*
 -------------------------------------
 Author:
 ID:
 Email:
 -------------------------------------
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#define True 1
#define False 0

typedef struct {
	unsigned long PID; // process id and it is 10 digit
	unsigned int time; // requested processing time or the service time
	unsigned int arrival; // arrival time
} Process;

Process* create_process(long p_pid, int p_time, int arrival_time);

void destroy_process(Process **p);

void print_process(Process *p);
Process* copy_process(Process *p1);

void get_process_info(Process *p, char *info);

int is_equal_process(Process *p1, Process *p2);

unsigned long get_UPID();


#endif /* PROCESS_H_ */
