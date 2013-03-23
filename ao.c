#include "ao.h"


AO *init_AO(int num) {
	AO *ao = malloc(sizeof(AO) + num * sizeof(Task *));
	ao->filesize = 0;
	ao->support_range = false;
	ao->max_tasks = num - 1; // start with 0
	for (int i = 0; i <= ao->max_tasks; i++)
		ao->tasks[i] = NULL;
	return ao;
}


void free_AO(AO *ao) {
	for (int i = 0; i <= ao->max_tasks; i++)
		if (ao->tasks[i])
			free_Task(ao->tasks[i]);
	free(ao);
}



int new_Task(AO *ao, int range_type, unsigned long start, unsigned long stop) {
	for (int i = 0; i <= ao->max_tasks; i++) {
		if (ao->tasks[i] == NULL) {
			ao->tasks[i] = init_Task(range_type, start, stop);
			ao->tasks[i]->taskid = i;
			return i;
		}
	}
	fprintf(stderr, "can not create more tasks\n");
	return -1;
}



void del_Task(AO *ao, Task *task) {
	ao->tasks[task->taskid] = NULL;
	free_Task(task);
}



Task *init_Task(int range_type, unsigned long start, unsigned long stop) {
	Task *task = malloc(sizeof(Task));
	task->url = init_Url();
	task->request = init_Request();
	task->response = init_Response();
	task->range_type = range_type;
	task->start = start;
	task->stop = stop;
	return task;
}



void clear_Task(Task *task) {
	free_Url(task->url);
	free_Request(task->request);
	free_Response(task->response);
	task->url = init_Url();
	task->request = init_Request();
	task->response = init_Response();
}



void free_Task(Task *task) {
	free_Url(task->url);
	free_Request(task->request);
	free_Response(task->response);
	free(task);
}


/////////


char *dynamic_copy(char *src, size_t src_size) {
	char *dest = malloc(sizeof(char) * (src_size + 1));
	memcpy(dest, src, src_size);
	dest[src_size] = '\0';
	return dest;
}



void static_copy(char *dest, size_t dest_size, char *src, size_t src_size) {
	if (dest_size >= src_size) {
		memcpy(dest, src, src_size);
		dest[src_size] = '\0';
	} else {
		memcpy(dest, src, dest_size);
		dest[dest_size] = '\0';
	}
}
