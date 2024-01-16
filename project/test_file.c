#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

void test_FIFO();
void test_SJF();
void test_RR();
int read_processes(char*, Data*);

int main() {
	setbuf(stdout, NULL);
	// test_FIFO();
	test_SJF();
	// test_RR();
	// printf("Testing for LIFO Scheduler:\n");
	// dispatcher("LIFO", NULL, 2);
	return 0;
}

void test_FIFO() {
	printf("------------------------------\n");
	printf("Start: Testing FIFO Scheduler\n\n");

	Data *array = NULL;
	int size = 0;

	printf("First File:\n");
	array = (Data*) malloc(sizeof(Data) * 30);
	size = read_processes("processes1.txt", array);
	dispatcher("FIFO", array, size);
	free(array);
	array = NULL;
	printf("\n");

	printf("Second File:\n");
	array = (Data*) malloc(sizeof(Data) * 30);
	size = read_processes("processes2.txt", array);
	dispatcher("FIFO", array, size);
	free(array);
	array = NULL;
	printf("\n");

	printf("------------------------------\n");
	printf("End of Testing FIFO Scheduler\n\n");
	return;
}

void test_SJF() {
	printf("------------------------------\n");
	printf("Start: Testing SJF Scheduler\n\n");

	Data *array = NULL;
	int size = 0;

	printf("First File:\n");
	array = (Data*) malloc(sizeof(Data) * 30);
	size = read_processes("processes1.txt", array);
	dispatcher("SJF", array, size);
	free(array);
	array = NULL;
	printf("\n");

	printf("Second File:\n");
	array = (Data*) malloc(sizeof(Data) * 30);
	size = read_processes("processes2.txt", array);
	dispatcher("SJF", array, size);
	free(array);
	array = NULL;
	printf("\n");

	printf("------------------------------\n");
	printf("End of Testing SJF Scheduler\n\n");
	return;
}

void test_RR() {
	printf("------------------------------\n");
	printf("Start: Testing RR Scheduler\n\n");

	Data *array = NULL;
	int size = 0;

	printf("First File:\n");
	array = (Data*) malloc(sizeof(Data) * 30);
	size = read_processes("processes1.txt", array);
	dispatcher("RR5", array, size);
	free(array);
	array = NULL;
	printf("\n");

	printf("Second File:\n");
	array = (Data*) malloc(sizeof(Data) * 30);
	size = read_processes("processes2.txt", array);
	dispatcher("RR3", array, size);
	free(array);
	array = NULL;
	printf("\n");

	printf("------------------------------\n");
	printf("End of Testing RR Scheduler\n\n");
	return;
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
