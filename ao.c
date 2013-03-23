#include "ao.h"


AO *init_ao(int num) {
	AO *ao = malloc(sizeof(AO) + num * sizeof(Task *));
	ao->filesize = 0;
	ao->filename[0] = '\0';
	ao->url[0] = '\0';
	ao->use_task = true;
	ao->max_task = num - 1; // start with 0
	ao->used_task = 0;
	for (int i = 0; i < num; i++)
		ao->tasks[i] = NULL;
	return ao;
}


void free_ao(AO *ao) {
	int i;
	for (i = 0; i <= ao->used_task; i++)
		if (ao->tasks[i])
			free_task(ao->tasks[i]);
	free(ao);
}



int new_task(AO *ao, int type, unsigned long start, unsigned long stop) {
	int i = 0;
	if (ao->used_task == ao->max_task) {
		fprintf(stderr, "reach the limit\n");
		return -1;
	} else {
		for (i = 0; i<= ao->max_task; i++) {
			if (ao->tasks[i] == NULL)
				break;
		}
		ao->used_task++;
		ao->tasks[i] = init_task(type, start, stop);
		return i;
	}
}



Task *init_task(int type, unsigned long start, unsigned long stop) {
	Task *task = malloc(sizeof(Task));
	task->url = init_url();
	task->request = init_request();
	task->response = init_response();
	task->range_type = type;
	task->start = start;
	task->stop = stop;
	return task;
}



void clear_task(Task *task) {
	free_url(task->url);
	free_request(task->request);
	free_response(task->response);
	task->url = init_url();
	task->request = init_request();
	task->response = init_response();
}



void free_task(Task *task) {
	free_url(task->url);
	free_request(task->request);
	free_response(task->response);
	free(task);
}



char *copy_str(char *src, size_t len) {
	char *dest = malloc(sizeof(char) * (len + 1));
	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest;
}
