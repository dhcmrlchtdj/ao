#include "ao.h"


environ_t env;

int main(int argc, char *argv[]) {
	initial_environ(&env);
	parse_options(&env, argc, argv);
	start_download(&env);
	destroy_environ(&env);
}


///////////////////////////////////////////////////////////////////////////////


void initial_environ(environ_t *env) {
	memset(env, 0, sizeof(environ_t));

	env->partition = DEFAULT_PARTITION;

	env->tasks = calloc(env->partition, sizeof(task_t));
	assert(env->tasks != NULL);

	env->epoll_fd = epoll_create1(0);
	assert(env->epoll_fd != -1);

	env->timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
	assert(env->timer_fd != -1);

	//set_alarm(env->timer_fd, 200000); // 200,000 nanosecond == 0.2 second
	//use gettimeofday to calculate time
}


void destroy_environ(environ_t *env) {
	close(env->epoll_fd);
	close(env->timer_fd);
	del_url(env->url);
	free(env->tasks);
}


///////////////////////////////////////////////////////////////////////////////


void initial_task(task_t *task, off_t start_pos, off_t stop_pos) {
	task->url = env.url;
	create_connection(task);
	task->todo = wait_connect;
	task->todo(task);

	return;
	//task->socket_fd = tcp_conn(request->url);

	//set_range(request, start_pos, stop_pos);
	//request2str(request, task->request);
	//task->remain = strlen(task->request);

	//task->current = start_pos;

	//task->redirection = MAX_REDIRECTION;

	//task->data = new_data(start_pos);
}

void destroy_task(task_t *task) {
	return;
}
