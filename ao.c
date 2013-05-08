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
	create_connection(task);
	task->event.data.fd = task->socket_fd;
	task->event.events = EPOLLOUT;
	task->todo = wait_connect;

	task->url = env.url;
	task->start = start_pos;
	task->stop = stop_pos;
	task->redirection = MAX_REDIRECTION;

	task->request = new_request();
	task_update_request(task);

	task->response = new_response();
}



void destroy_task(task_t *task) {
	del_request(task->request);
	del_response(task->response);
	shutdown(task->socket_fd, SHUT_RDWR);
	close(task->socket_fd);
}



void task_update_request(task_t *task) {
	task->request->url = task->url;
	gen_request_header(task->request);
	set_range(task->request, task->start, task->stop);
	request2string(task->request);
}



void task_prepare_redirection(task_t *task) {
	if (task->redirection-- == 0) {
		fprintf(stderr, "[ao] redirection too many times.\n");
		exit(EXIT_FAILURE);
	}

	char *url = get_header(task->response->hf, "Location");
	if (url == NULL) {
		fprintf(stderr, "[ao] location not found.\n");
		exit(EXIT_FAILURE);
	}
	if (task->redirection != 4) del_url(task->url); // not first redirection
	task->url = parse_url(url);

	shutdown(task->socket_fd, SHUT_RDWR);
	close(task->socket_fd);

	create_connection(task);
	task->event.data.fd = task->socket_fd;
	task->event.events = EPOLLOUT;
	task->todo = wait_connect;

	task_update_request(task);

	del_header_field(task->response->hf);
	task->response->hf = NULL;
}
