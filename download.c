#include "ao.h"


void start_download(environ_t *env) {
	_dl_prepare(env);
}

void _dl_prepare(environ_t *env) {
	task_t *task = &env->tasks[0];
	initial_task(task, 0, 1);



	return;
}



/*
void _get_response_header(task_t *task, int epfd) {
//void _dl_prepare(task_t *task, int epfd) {
	ssize_t size;
	struct epoll_event ev, events;
	ev.data.fd = task->socket_fd;

	// FIXME
	// nonblocking connect
	ev.events = EPOLLOUT;
	epoll_ctl(epfd, EPOLL_CTL_ADD, task->socket_fd, &ev);
	while (1) {
		epoll_wait(epfd, &events, 1, -1);
		if ((events.events & EPOLLERR) || (events.events & EPOLLHUP)) {
			fprintf(stderr, "[ao] epoll error.\n");
			shutdown(task->socket_fd, SHUT_RDWR);
			close(task->socket_fd);
			exit(EXIT_FAILURE);
		} else if (events.events & EPOLLOUT) {
			size = Send(task->socket_fd,
					task->request + task->offset,
					task->remain);
			task->offset += size;
			task->remain -= size;
			if (task->remain == 0) {
				ev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_MOD, task->socket_fd, &ev);
				// reset offset and remain
				task->offset = 0;
				task->remain = RECV_SIZE;
			}
		} else if (events.events & EPOLLIN) {
			size = Recv(task->socket_fd,
					task->data->data + task->offset,
					task->remain);
			if (size == -1) continue;
			// FIXME
			// response length
			task->offset += size;
			task->remain -= size;
			if (size == 0 || task->remain == 0) {
				epoll_ctl(epfd, EPOLL_CTL_DEL, task->socket_fd, NULL);
				shutdown(task->socket_fd, SHUT_RDWR);
				close(task->socket_fd);
				break;
			}
		}
	}
}
*/
