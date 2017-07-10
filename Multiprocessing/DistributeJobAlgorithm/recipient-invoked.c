#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include "task_list.h"

#define PROCESSOR_IS_UNDERLOADED  (task_lists[i] == NULL || \
									task_lists[i]->task == NULL || \
									task_lists[i]->task->start_time > now)


int processArgs (int argc, const char** argv, int* N) {
	if (argc == 1) {
		printf("Set default number of processors: 4\n");
		*N = 4;
		return 0;
	}
	if (argc == 2) {
		errno = 0;
		int res = atoi(argv[1]);
		if(errno == 0) {
			printf("Set number of processors: %d\n", res);
			*N = res;
			return 0;
		}
	}
	printf("Error: invalid arguments\nUsage: %s <number of processors>\n", argv[1]);
	return -1;
}

task_t* findJob(int now, int processor, task_list_t** task_lists, int N) {

	task_t* next_task;
	task_t* curr_task;
	task_list_t* tl;
	for (int i = 0; i < N; i++) {
		printf("Processor %d asks %d for job... ", processor, i);
		tl = task_lists[i];
		if (tl != NULL && tl->next != NULL) {
			next_task = tl->next->task;
			curr_task = tl->task;
			if (next_task->start_time < now && curr_task->start_time + curr_task->required_CPU_time > now) { // processor i is overloaded
				printf("%s\n", "and gets it.");
				task_t* got_task = tl_get_task(tl);
				got_task->start_time = now;
				return got_task;
			}
		}
		printf("%s\n", "and doesn't get it.");
	}
	return NULL;
}

int checkUnderloaded(int now, int processor, task_list_t** task_lists, int N) {
	if (task_lists[processor] == NULL || task_lists[processor]->task == NULL || task_lists[processor]->task->start_time > now) {
		task_t* new_task = findJob(now, processor, task_lists, N);
		if (new_task != NULL)
		{
			task_lists[processor] = tl_add(task_lists[processor], new_task);
			printf("now it is not underloaded\n");
			return 0; //now it is not underloaded
		}
		printf("underloaded\n");
		return 1; //underloaded
	}
	printf("not underloaded\n");
	return 0; //not underloaded
}

int main (int argc, const char** argv) {
	int N; //number of processors
	if (processArgs(argc, argv, &N) != 0) {
		return -1;
	}

	FILE* in = fopen("input.txt", "r");
	if (in == NULL) {
		perror(NULL);
		return -1;
	}

	task_list_t** task_lists = calloc(N,sizeof(task_list_t*));
	if (task_lists == NULL) {
		perror(NULL);
		return -1;
	}

	int creating_processor;
	int start_time;
	int required_CPU_time;
	task_t *task;
	task_list_t* tl;
	while(fscanf(in, "(%d, %d, %d)\n", &creating_processor, &start_time, &required_CPU_time) != EOF) {
		printf("(%d, %d, %d)\n", creating_processor, start_time, required_CPU_time);
		task = malloc(sizeof(task));
		if (task == NULL) {
			perror(NULL);
			return -1;
		}
		task->creating_processor = creating_processor;
		task->start_time = start_time;
		task->required_CPU_time = required_CPU_time;
		tl = tl_add(task_lists[creating_processor], task);
		if (tl == NULL) {
			perror(NULL);
			return -1;
		}
		task_lists[creating_processor] = tl;
		printf("Added task\n");
	}


	int now = 0;
	int next_end = INT_MAX;
	int end_time;
	int underloaded;
	do {
		next_end = INT_MAX;
		for (int i = 0; i < N; i++) {
			printf("entering loop; i = %d\n", i);
			if (task_lists[i] != NULL && 
				task_lists[i]->task != NULL && 
				task_lists[i]->task->start_time + task_lists[i]->task->required_CPU_time == now) {
				printf("end_time == now == %d\n", now);
				task_lists[i] = tl_pop(task_lists[i]);
			}
			underloaded = checkUnderloaded(now, i, task_lists, N);
			printf("checked underloaded = %d\n", underloaded);
			if (underloaded) {
				continue;
			}
			task = task_lists[i]->task;
			end_time = task->start_time + task->required_CPU_time;
			if (end_time < next_end) {
				printf("end_time < next_end\n");
				next_end = end_time;
			}
		}
		printf("now: %d, next_end: %d\n", now, next_end);
		now = next_end;
	} while (next_end < INT_MAX);

	fclose(in);
	for (int i = 0; i < N; i++) {
		tl_free(task_lists[i]);
	}
	free(task_lists);
}