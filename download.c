#include "ao.h"


void start_download(environ_t *env) {
	dl_prepare(env);
}



void dl_prepare(environ_t *env) {
	task_t *task = &env->tasks[0];
	initial_task(task, 0, 1);
	dl_get_response(env->epoll_fd, task);
	// whether support range
	// whether file existed
	// whether log file existed
	return;
}



void dl_get_response(int epfd, task_t *task) {
	int status;
	struct epoll_event ev;
	epoll_ctl(epfd, EPOLL_CTL_ADD, task->socket_fd, &task->event);

	while (1) {
		epoll_wait(epfd, &ev, 1, -1);
		if ((ev.events & EPOLLERR) || (ev.events & EPOLLHUP)) {
			fprintf(stderr, "[ao] epoll error\n");
			exit(EXIT_FAILURE);
		} else if (ev.events & EPOLLOUT) {
			// wait connect && send request
			status = task->todo(task);
			if (status == 0)
				epoll_ctl(epfd, EPOLL_CTL_MOD, task->socket_fd, &task->event);
		} else if (ev.events & EPOLLIN) {
			// recv response
			status = task->todo(task);
			if (status == 0) {
				string2response(task->response);
				switch (task->response->status[0]) {
					case '2': // 2xx
						return;
					case '3': // 3xx
						task_prepare_redirection(task);
						epoll_ctl(epfd, EPOLL_CTL_MOD,
								task->socket_fd, &task->event);
					default: // 1xx, 4xx, 5xx
						fprintf(stderr, "[ao] http error: '%s'\n",
								task->response->status);
						exit(EXIT_FAILURE);
				}
			}
		}
	}
}


/*
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
