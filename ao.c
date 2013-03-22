#include "ao.h"


ao_t *init_ao_t(int num) {
	ao_t *ao = malloc(sizeof(ao_t) + num * sizeof(tasklet_t *));
	return ao;
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
