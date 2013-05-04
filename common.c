#include "ao.h"


char *dynamic_copy(char *src, size_t src_size) {
	char *dest = malloc(sizeof(char) * (src_size + 1));
	assert(dest != NULL);
	memcpy(dest, src, src_size);
	dest[src_size] = '\0';
	return dest;
}



void static_copy(char *dest, size_t dest_size, char *src, size_t src_size) {
	if (dest_size > src_size) {
		memcpy(dest, src, src_size);
		dest[src_size] = '\0';
	} else {

		dest[dest_size] = '\0';
	}
}


//////////////////////////////


environ_t *new_environ(void) {
	environ_t *new = calloc(1, sizeof(environ_t));
	assert(new != NULL);
	return new;
}



void del_environ(environ_t *env) {
	/*close(env->epoll_fd);*/
	/*close(env->timer_fd);*/
	del_url(env->url);
	free(env->tasks);
	free(env);
}



void environ_update(environ_t* env) {
	if (env->partition == 0)
		env->partition = PARTITION;

	env->tasks = calloc(env->partition, sizeof(task_t));
	assert(env->tasks != NULL);

	env->epoll_fd = epoll_create1(0);
	assert(env->epoll_fd != -1);

	env->timer_fd = timerfd_create(CLOCK_MONOTONIC, 0); //TFD_NONBLOCK);
	assert(env->timer_fd != -1);

	//set_alarm(env->timer_fd, 200000); // 200,000 nanosecond == 0.2 second
	//use gettimeofday to calculate time
}


//////////////////////////////


void initial_task(task_t *task, request_t *request,
		off_t start_pos, off_t stop_pos) {
	task->socket_fd = tcp_conn(request->url);

	task->response = calloc(RECV_SIZE, sizeof(char));

	set_range(request, start_pos, stop_pos);
	request2str(request, task->request);
	task->remain = strlen(task->request);

	task->current = start_pos;
}



void destory_task(task_t *task) {
	return;
}


//////////////////////////////


void set_alarm(int timer_fd, long nsec) {
	// {0, 0} means alarm once
	// {0, nsec} means after nsec nanoseconds
	struct itimerspec interval = { {0, 0}, {0, nsec} };
	timerfd_settime(timer_fd, 0, &interval, NULL);
}



long delta_time(struct timeval *t1, struct timeval *t2) {
	long sec = t2->tv_sec - t1->tv_sec;
	long usec = t2->tv_usec - t1->tv_usec;
	return ((sec * 1000000 + usec) / 1000);
}
