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
	env->epoll_fd = Epoll_create();
	env->timer_fd = Timerfd_create();
	Sigemptyset(&env->sigset);
	Sigaddset(&env->sigset, SIGINT);
	Sigaddset(&env->sigset, SIGTERM);
	env->signal_fd = Signalfd(&env->sigset);
}



void destroy_environ(environ_t *env) {
	for (int i = 0; i < env->partition; i++)
		destroy_task(&env->tasks[i]);
	free(env->tasks);
	Close(env->file_fd);
	Close(env->epoll_fd);
	Close(env->timer_fd);
	Close(env->signal_fd);
}



void environ_update_by_log(environ_t *env) {
	env->epoll_fd = Epoll_create();
	env->timer_fd = Timerfd_create();
	env->signal_fd = Signalfd(&env->sigset);
}



task_t *get_task_by_fd(environ_t *env, int fd) {
	for (int i = 0; i < env->partition; i++)
		if (env->tasks[i].socket_fd == fd)
			return &env->tasks[i];
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////


task_t *new_task(off_t start, off_t stop) {
	task_t *task = Calloc(1, sizeof(task_t));
	initial_task(task, start, stop);
	return task;
}



void del_task(task_t *task) {
	destroy_task(task);
	free(task);
}



void initial_task(task_t *task, off_t start, off_t stop) {
	task->start = start;
	task->stop = stop;
	task_update_pointer(task);
}



void destroy_task(task_t *task) {
	if (task->redirection != MAX_REDIRECTION) del_url(task->url);
	del_request(task->request);
	del_response(task->response);
	Shutdown(task->socket_fd, SHUT_RDWR);
	Close(task->socket_fd);
}



void task_update_request(task_t *task) {
	task->request->url = task->url;
	gen_request_header(task->request);
	set_range(task->request, task->start, task->stop);
	request2string(task->request);
}



void task_prepare_redirection(task_t *task) {
	char *url = get_header(task->response->hf, "Location");
	if (url == NULL) {
		fprintf(stderr, "[ao] redirction error: "
				"Location not found.\n%s\n", task->response->data);
		exit(EXIT_FAILURE);
	}

	if (task->redirection == 0) {
		fprintf(stderr, "[ao] redirection error: "
				"redirection too many times.\n%s\n", url);
		exit(EXIT_FAILURE);
	}

	if (task->redirection-- != MAX_REDIRECTION)
		del_url(task->url); // not first redirection
	task->url = new_url();
	parse_url(task->url, url);

	Shutdown(task->socket_fd, SHUT_RDWR);
	Close(task->socket_fd);

	create_connection(task);
	task->event.data.fd = task->socket_fd;
	task->event.events = EPOLLOUT;
	task->todo = wait_connect;

	task_update_request(task);

	del_header_field(task->response->hf);
	task->response->hf = NULL;

	task->offset = 0;
	task->remain = RECV_SIZE;
}



void task_update_pointer(task_t *task) {
	task->redirection = MAX_REDIRECTION;
	task->url = &env.url;
	create_connection(task);
	task->event.data.fd = task->socket_fd;
	task->event.events = EPOLLOUT;
	task->todo = wait_connect;

	task->request = new_request();
	task_update_request(task);

	task->response = new_response();
}
