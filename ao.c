#include "ao.h"


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

	env->timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
	assert(env->timer_fd != -1);

	//set_alarm(env->timer_fd, 200000); // 200,000 nanosecond == 0.2 second
	//use gettimeofday to calculate time
}


//////////////////////////////


void initial_task(task_t *task, request_t *request,
		off_t start_pos, off_t stop_pos) {
	task->socket_fd = tcp_conn(request->url);

	set_range(request, start_pos, stop_pos);
	request2str(request, task->request);
	task->remain = strlen(task->request);

	task->current = start_pos;

	task->redirection = MAX_REDIRECTION;

	task->data = new_data(start_pos);
}



void destory_task(task_t *task) {
	return;
}


//////////////////////////////


data_t *new_data(off_t pos) {
	data_t *new = malloc(sizeof(data_t));
	assert(new != NULL);
	new->pos = pos;
	new->data = calloc(RECV_SIZE, sizeof(char));
	assert(new->data != NULL);
	return new;
}
