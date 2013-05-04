#include "ao.h"


void start_download(environ_t *env) {
	request_t *req = new_request(env->url);

	task_t *task = &env->tasks[0];
	initial_task(task, req, 0, 1);
	_dl_prepare(task, env->epoll_fd);
	printf("%s\n", task->response);
}



void _dl_prepare(task_t *task, int epfd) {
	ssize_t size;
	struct epoll_event ev, events;

	ev.data.fd = task->socket_fd;
	ev.events = EPOLLOUT;
	epoll_ctl(epfd, EPOLL_CTL_ADD, task->socket_fd, &ev);

	// FIXME
	// nonblocking connect
	while (1) {
		epoll_wait(epfd, &events, 1, -1);
		if ((ev.events & EPOLLERR) || (ev.events & EPOLLHUP)) {
			fprintf(stderr, "[ao] epoll error.\n");
			close(ev.data.fd);
			break;

		} else if (ev.events & EPOLLOUT) {
			size = _send(task->socket_fd,
					task->request + task->offset,
					task->remain);
			if (size == -1) continue;
			if (size == 0) {
				ev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_MOD, task->socket_fd, &ev);
				task->offset = 0; // reset offset
				task->remain = RECV_SIZE;
			} else {
				task->offset += size;
				task->remain -= size;
			}

		} else if (ev.events & EPOLLIN) {
			size = _recv(task->socket_fd,
				   	task->response + task->offset,
				   	task->remain);
			if (size == -1) continue;
			if (size == 0) {
				epoll_ctl(epfd, EPOLL_CTL_DEL, task->socket_fd, NULL);
				/*epoll_ctl(epfd, EPOLL_CTL_MOD, task->socket_fd, */
				/*break;*/
				printf("======ok\n");
				printf("%s\n", task->response);
			} else {
				printf("size: %zd\n", size);
				task->offset += size;
				task->remain -= size;
				if (task->remain <= 0)
					break;
			}
		}
	}
}
