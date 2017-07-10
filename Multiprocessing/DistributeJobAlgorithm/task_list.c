#include "task_list.h"
#include <stdlib.h>


task_list_t *tl_new(task_t* task, task_list_t* next) {
	task_list_t *new  = malloc(sizeof(task_list_t));
	if (new == NULL) {
		return NULL;
	}
	new->next = next;
	new->task= task;
	return new;
}

task_list_t* tl_add(task_list_t* tl, task_t*task) {
	if (tl == NULL) {
		return tl_new(task, NULL);
	}
	if (tl->task->start_time > task->start_time) {
		return tl_new(task, tl);
	}

	task_list_t *curr = tl;
	task_list_t * next = tl->next;
	while (next && next->task->start_time < task->start_time) {
		curr = next;
		next = curr->next;
	}

	if (curr->next == NULL) {
		curr->next = tl_new(task, NULL);
		return curr->next ? tl : NULL;
	}
	if (curr->next->task->start_time > task->start_time) {
		task_list_t *new  = tl_new(task, curr->next);
		curr->next = new;
		return new ? tl : NULL;
	}
	return NULL;
}

task_list_t* tl_pop(task_list_t* tl) {
	task_list_t* new_head = tl->next;
	free(tl->task);
	free(tl);
	return new_head;
}

task_t* tl_get_task(task_list_t* tl) {
	if (tl == NULL || tl->next == NULL) {
		return NULL;
	}
	task_t* t = tl->next->task;
	task_list_t* tmp = tl->next;
	tl->next = tmp->next;
	free(tmp);
	return t;
}

void tl_free(task_list_t* tl) {
	task_list_t *curr = tl;
	task_list_t *next;
	while(curr) {
		free(curr->task);
		next = curr->next;
		free(curr);
		curr = next;
	}
}