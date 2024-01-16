# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <assert.h>
#include <mysql.h>
# include "scheduler.h"
#include <limits.h> // For INT_MAX

MYSQL *db_connection();
void log_process_action();
void insert_update_process();

/*---------------------Task 1 -----------------------------*/
void dispatcher(char *type, Data *processes, const int size) {
    if (processes == NULL) {
        printf("Error(schedule): Unsupported scheduler type\n");
        return;
    }

    // Createing a priority queue
    pQueue *q = create_pqueue((size + size / 2), 'L'); // Choose 'L' or 'H' based on your design
    for (int i = 0; i < size; i++) {
        insert_pqueue2(q, &processes[i]);
    }

    // Checking for FIFO or SJF scheduler types
    if (strcmp(type, "FIFO") == 0) {
        schedule_FIFO(q);
    } else if (strcmp(type, "SJF") == 0) {
        schedule_SJF(q);
    } else if (strncmp(type, "RR", 2) == 0) { // Check if type starts with "RR"
        // Extract the period from the type string
        int period = atoi(type + 2);
        if (period > 0) {
            schedule_RR(q, period);
        } else {
            printf("Error(schedule): Invalid period for RR scheduler\n");
        }
    } else {
        printf("Error(schedule): Unsupported scheduler type\n");
    }

    destroy_pqueue(&q); // Free the queue
}


/*---------------------Task 2 -----------------------------*/
void schedule_FIFO(pQueue *q) {
	int timer = 0;
	printf("[Timer:%d]: Starting FIFO Scheduler\n", timer);

    // Establishing Database Connection

    MYSQL *con = db_connection("localhost", "username", "password", "scheduler_database");
    if (con == NULL) {
        printf("connection cannot be established\n");
        return;
    }
    

	timer++;
	while (peek_pqueue2(q)->arrival > timer) {
		printf("[Timer:%d]: Idle\n", timer);
        log_process_action(con, -1, "FIFO", "Idle", timer); // Loging idle action
		timer++;
	}
   

	while (!is_empty_pqueue(q)) {
		if (peek_pqueue2(q)->arrival > timer) {
			printf("[Timer:%d]: Idle\n", timer);
            log_process_action(con, -1, "FIFO", "Idle", timer);
			timer++;
			continue;
		}
		Data *d = remove_pqueue2(q);
		int rounds = d->time;

		printf("Fetching Process: ");
		print_data(d);
		printf("\n");

        insert_update_process(con, d->PID, d->arrival, d->time);

        // Loging fetched process
        log_process_action(con, d->PID, "FIFO", "Fetch", timer);

		for (int i = 0; i < rounds; i++) {
			printf("[Timer:%d]: ", timer);
			print_data(d);
			printf("\n");

            // Loging executing process
            log_process_action(con, d->PID, "FIFO", "Execute", timer);
			timer++;
		}

	}

	printf("[Timer:%d]: Closing FIFO Scheduler\n", timer);
    log_process_action(con, -1, "FIFO", "End", timer);

    // Closing the database connection
    mysql_close(con);

	return;
}

/*---------------------Task 3 -----------------------------*/


void schedule_SJF(pQueue *q) {
    int timer = 0;
    printf("[Timer:%d]: Starting SJF Scheduler\n", timer);

    // Establishing Database Connection

    MYSQL *con = db_connection("localhost", "username", "password", "scheduler_database");
    if (con == NULL) {
        printf("connection cannot be established\n");
        return;
    }

    timer++;

    while (!is_empty_pqueue(q)) {
        // Waiting until a process has arrived
        while (!is_empty_pqueue(q) && peek_pqueue2(q)->arrival > timer) {
            printf("[Timer:%d]: Idle\n", timer);
            log_process_action(con, -1, "SJF", "Idle", timer); // Loging idle action
            timer++;
        }

        // Temporary buffering to store processes that have arrived
        Data *tempBuffer = malloc(sizeof(Data) * len_pqueue(q));
        int tempSize = 0;

        // Moving all arrived processes to temporary buffer
        while (!is_empty_pqueue(q) && peek_pqueue2(q)->arrival <= timer) {
            Data *arrivedProcess = remove_pqueue2(q);
            tempBuffer[tempSize++] = *arrivedProcess;
            destroy_data(&arrivedProcess); // Freeing the removed process
        }

        // Finding the shortest job in the temporary buffer
        int shortestIndex = 0;
        int shortestTime = INT_MAX;
        for (int i = 0; i < tempSize; i++) {
            if (tempBuffer[i].time < shortestTime) {
                shortestTime = tempBuffer[i].time;
                shortestIndex = i;
            }
        }

        // Processing the shortest job
        Data shortestJob = tempBuffer[shortestIndex];
        printf("Fetching Process: ");
        print_data(&shortestJob);
        printf("\n");

        insert_update_process(con, shortestJob.PID, shortestJob.arrival, shortestJob.time);


        // Log fetching process
        log_process_action(con, shortestJob.PID, "SJF", "Fetch", timer);

        for (int i = 0; i < shortestJob.time; i++) {
            printf("[Timer:%d]:", timer);
            print_data(&shortestJob);
            printf("\n");

            // Loging fetched process
            log_process_action(con, shortestJob.PID, "SJF", "Execute", timer);
            timer++;
        }

        // Reinserting remaining processes back into the queue
        for (int i = 0; i < tempSize; i++) {
            if (i != shortestIndex) {
                insert_pqueue2(q, &tempBuffer[i]);
            }
        }

        free(tempBuffer); // Freeing the temporary buffer
    }

        printf("[Timer:%d]: Closing SJF Scheduler\n", timer);
        // Loging end of SF Scheduler
        log_process_action(con, -1, "SJF", "End", timer);

        // Closing the database connection
        mysql_close(con);
}


/*---------------------Task 4 -----------------------------*/

void schedule_RR(pQueue *q, int period) {
    char schedulerType[10];
    sprintf(schedulerType, "RR%d", period);  

    int timer = 0;
    printf("[Timer:%d]: Starting RR%d Scheduler \n", timer, period);

    // Establishing Database Connection
    MYSQL *con = db_connection("localhost", "username", "password", "scheduler_database");
    if (con == NULL) {
        printf("Connection cannot be established\n");
        return;
    }


    timer++;

    while (!is_empty_pqueue(q)) {
        if (peek_pqueue2(q)->arrival > timer) {
            printf("[Timer:%d]: Idle\n", timer);
            log_process_action(con, -1, schedulerType, "Idle", timer);

            timer++;
            continue;
        }

        Data *currentProcess = remove_pqueue2(q);
        printf("Fetching Process: ");
        print_data(currentProcess);
        printf("\n");


        int serviceTime = (currentProcess->time < period) ? currentProcess->time : period;

        insert_update_process(con, currentProcess->PID, currentProcess->arrival, currentProcess->time);

        log_process_action(con, currentProcess->PID, schedulerType, "Fetch", timer);

        for (int i = 0; i < serviceTime; i++) {
            printf("[Timer:%d]: ", timer);
            print_data(currentProcess);
            printf("\n");
            log_process_action(con, currentProcess->PID, schedulerType, "Fetch", timer);
            timer++;
        }

        currentProcess->time -= serviceTime;
        if (currentProcess->time > 0) {
            currentProcess->arrival = timer;
            insert_pqueue2(q, currentProcess);
        } else {
            destroy_data(&currentProcess); // Freeing the completed process
        }
    }

    printf("[Timer:%d]: Closing RR%d Scheduler \n", timer, period);
    // Loging end of SF Scheduler
    log_process_action(con, -1, schedulerType, "End", timer);

    // Closing the database connection
    mysql_close(con);
}

MYSQL *db_connection(const char *host, const char *user, const char *password, const char *dbname) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return NULL;
    }

    if (mysql_real_connect(con, host, user, password, dbname, 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return NULL;
    }

    return con;
}

void log_process_action(MYSQL *con, int process_id, const char *scheduler_type, const char *action, int timer) {
    char query[256];
    sprintf(query, "INSERT INTO SchedulerLog (process_id, scheduler_type, action, time_stamp) VALUES (%d, '%s', '%s', FROM_UNIXTIME(%d))", process_id, scheduler_type, action, timer);
    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
    }
}

void insert_update_process(MYSQL *con, int process_id, int arrival_time, int execution_time) {
    char query[256];
    sprintf(query, "INSERT INTO Processes (process_id, arrival_time, execution_time) VALUES (%d, %d, %d) ON DUPLICATE KEY UPDATE arrival_time = VALUES(arrival_time), execution_time = VALUES(execution_time)", process_id, arrival_time, execution_time);

    if (mysql_query(con, query)) {
        fprintf(stderr, "Error inserting/updating process data: %s\n", mysql_error(con));
    }
}

