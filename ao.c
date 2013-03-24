#include "ao.h"


AO *init_AO(int num) {
	AO *ao = malloc(sizeof(AO) + num * sizeof(Task *));
	ao->filesize = 0;
	ao->max_tasks = num;
	for (int i = 0; i < ao->max_tasks; i++)
		ao->tasks[i] = NULL;
	return ao;
}


void free_AO(AO *ao) {
	for (int i = 0; i < ao->max_tasks; i++)
		if (ao->tasks[i])
			free_Task(ao->tasks[i]);
	free(ao);
}


void setup_Tasks(AO *ao) {
	unsigned long block_size = ao->filesize / ao->max_tasks;
	unsigned long start = 0, stop = 0;
	for (int i = 0; i < ao->max_tasks - 1; i++) {
		start = stop;
		stop += block_size;
		new_Task(ao, true, start, stop - 1);
		printf("%lu-%lu\n", start, stop -1);
	}
	new_Task(ao, true, stop, ao->filesize);
	printf("%lu-%lu\n", stop, ao->filesize);
}



int new_Task(AO *ao, bool range, unsigned long start, unsigned long stop) {
	for (int i = 0; i < ao->max_tasks; i++) {
		if (ao->tasks[i] == NULL) {
			ao->tasks[i] = init_Task(range, start, stop);
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



Task *init_Task(bool range, unsigned long start, unsigned long stop) {
	Task *task = malloc(sizeof(Task));
	task->url = init_Url();
	task->request = init_Request();
	task->response = init_Response();
	task->range = range;
	task->start = start;
	task->stop = stop;
	task->received = 0;
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
	if (dest_size >= src_size + 1) {
		memcpy(dest, src, src_size + 1);
	} else {
		memcpy(dest, src, dest_size - 1);
		dest[dest_size] = '\0';
	}
}
