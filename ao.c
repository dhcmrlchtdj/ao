#include "ao.h"


ao_t *init_ao_t(int num) {
	ao_t *ao = malloc(sizeof(ao_t) + num * sizeof(tasklet_t *));
	ao->task_count = num;
	ao->filesize = 0;
	ao->filename[0] = '\0';
	ao->url[0] = '\0';
	for (int i = 0; i < num; i++)
		ao->tasklets[i] = NULL;
	return ao;
}


void free_ao_t(ao_t *ao) {
	for (int i = 0; i < ao->task_count; i++)
		if (ao->tasklets[i])
			free_tasklet_t(ao->tasklets[i]);
	free(ao);
}



tasklet_t *init_tasklet_t(unsigned long start, unsigned long stop) {
	tasklet_t *tasklet = malloc(sizeof(tasklet_t));
	tasklet->url = init_url_t();
	tasklet->request = init_request_t();
	tasklet->response = init_response_t();
	tasklet->start = start;
	tasklet->stop = stop;
	return tasklet;
}



void clear_tasklet_t(tasklet_t *tasklet) {
	free_url_t(tasklet->url);
	free_request_t(tasklet->request);
	free_response_t(tasklet->response);
	tasklet->url = init_url_t();
	tasklet->request = init_request_t();
	tasklet->response = init_response_t();
}



void free_tasklet_t(tasklet_t *tasklet) {
	free_url_t(tasklet->url);
	free_request_t(tasklet->request);
	free_response_t(tasklet->response);
	free(tasklet);
}



char *copy_str(char *src, size_t len) {
	char *dest = malloc(sizeof(char) * (len + 1));
	memcpy(dest, src, len);
	dest[len] = '\0';
	return dest;
}
