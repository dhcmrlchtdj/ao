#include "ao.h"


void initial_env(env_t *env) {
	memset(env, 0, sizeof(env_t));
	env->has_log = false;
	env->task_num = THREAD_NUM;
}



task_t *init_task(bool add_range, ...) {
	task_t *task = malloc(sizeof(task_t));
	task->next = NULL;
	task->url = init_url();
	task->request = init_request();
	task->response = init_response();
	if (add_range) {
		task->add_range = true;
		va_list ap;
		va_start(ap, add_range);
		task->range_start = va_arg(ap, off_t);
		task->range_stop = va_arg(ap, off_t);
		va_end(ap);
	} else {
		task->add_range = false;
	}
	return task;
}

void update_task(task_t *task) {
	task->url = init_url();
	task->request = init_request();
	task->response = init_response();
	task->next = NULL;
}



void free_task(task_t *task) {
	shutdown(task->sockfd, SHUT_RDWR);
	close(task->sockfd);
	free_url(task->url);
	free_request(task->request);
	free_response(task->response);
	free(task);
}



void clear_task(task_t *task) {
	free_url(task->url);
	free_request(task->request);
	free_response(task->response);
	task->url = init_url();
	task->request = init_request();
	task->response = init_response();
}


/////////


char *dynamic_copy(char *src, size_t src_size) {
	char *dest = malloc(sizeof(char) * (src_size + 1));
	memcpy(dest, src, src_size);
	dest[src_size] = '\0';
	return dest;
}



void static_copy(char *dest, size_t dest_size, char *src, size_t src_size) {
	if (dest_size > src_size) {
		memcpy(dest, src, src_size);
		dest[src_size] = '\0';
	} else {
		memcpy(dest, src, dest_size - 1);
		dest[dest_size] = '\0';
	}
}



long delta_time(struct timeval *t1, struct timeval *t2) {
	long sec = t2->tv_sec - t1->tv_sec;
	long usec = t2->tv_usec - t1->tv_usec;
	return ((sec * 1000000 + usec) / 1000);
}