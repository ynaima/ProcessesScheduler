#include<assert.h>
#include<stdlib.h>
#include "data.h"
#include <stdio.h>

void print_data(Data *d) {
	print_process(d);
	return;
}
void destroy_data(Data **d) {
	destroy_process(d);
	return;
}
Data* copy_data(Data *d) {
	return copy_process(d);
}
/**
 * ------------------------------------------------------------
 * Parameters: 	d1: A pointer to a data struct (Data*)
 * 				d2: A pointer to a data struct (Data*)
 * Returns: 	1: If d1 is greater than d2
 * 				2: If d2 is greater than d1
 * 				0: If d1 == d2
 * Description:	compares two data structs(processes);
 *
 * Assert:		d1 && d2 is not NULL
 * ------------------------------------------------------------
 */
int compare_data(Data *d1, Data *d2) {
	assert(d1 != NULL && d2 != NULL);
	if (d1->arrival > d2->arrival) {
		return 1;
	} else if (d1->arrival < d2->arrival) {
		return 2;
	} else {
		if (d1->time > d2->time) {
			return 1;
		} else if (d1->time < d2->time) {
			return 2;
		} else {
			if (d1->PID > d2->PID) {
				return 1;
			} else if (d1->PID < d2->PID) {
				return 2;
			}

			else
				return 0;
		}

	}
}
int read_processes(char *fileName, Data *array) {
	FILE *inFile = NULL;
	inFile = fopen(fileName, "r");
	if (inFile == NULL) {
		printf("Error(read_processes): Cannot open file %s\n", fileName);
		return 0;
	}

	char line[40], tempArrival[7], tempPID[12], tempTime[7], *dummy;
	int marker1, marker2, i, factor, digit;
	unsigned int arrival, time;
	unsigned long pid;

	int counter = 0;
	while (fgets(line, sizeof(line), inFile) != NULL) {
		counter++;
		i = 0;

		while (line[i] != ']')
			i++;
		marker1 = i;
		for (i = 1; i < marker1; i++)
			tempArrival[i - 1] = line[i];
		arrival = (unsigned int) strtol(tempArrival, &dummy, 10);

		marker1 += 2;
		for (i = marker1; i < (marker1 + 10); i++)
			tempPID[i - marker1] = line[i];

		pid = 0;
		factor = 1000000000;
		for (i = 0; i < 10; i++) {
			digit = tempPID[i] - '0';
			pid += digit * factor;
			factor = factor / 10;
		}

		marker1 += 11;
		i = marker1;
		while (line[i] != ')')
			i++;
		marker2 = i;

		for (i = marker1; i < marker2; i++)
			tempTime[i - marker1] = line[i];
		tempTime[i - marker1] = '\0';

		time = (unsigned int) strtol(tempTime, &dummy, 10);
		array[counter - 1] = *create_process(pid, time, arrival);
	}
	fclose(inFile);
	return counter;
}


