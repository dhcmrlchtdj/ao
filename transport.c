#include "ao.h"


// wait for nonblokcing connect succeed
// event will keep wait EPOLLOUT
int wait_connect(task_t *task) {
	int value;
	socklen_t len = sizeof(value);
	Getsockopt(task->socket_fd, SOL_SOCKET, SO_ERROR, &value, &len);
	if (value == 0) {
		// connect succeed
		// task->event.events = EPOLLOUT;
		task->todo = send_request;
		task->offset = 0;
		task->remain = strlen(task->request->data);
	}
	return 1;
}



// 0 => goto next step
// 1 => keep writing
int send_request(task_t *task) {
	ssize_t size = Send(task->socket_fd,
			task->request->data + task->offset, task->remain);
	if (size == 0) { // finish
		task->offset = 0;
		task->remain = RECV_SIZE;
		task->flag = FLAG_RESPONSE_START;
		task->todo = recv_response;
		task->event.events = EPOLLIN;
		return 0;
	} else {
		if (size != -1) {
			task->offset += size;
			task->remain -= size;
		}
		return 1;
	}
}



// 0 => response got
// 1 => keep reading
int recv_response(task_t *task) {
	ssize_t size;
	while (1) {
		size = Recv(task->socket_fd,
				task->response->data + task->offset, 1);
		if (size == -1) { // EAGAIN
			return 1;
		} else if (size == 0) { // connect be closed before succeed ?
			fprintf(stderr, "[ao] some wrong on socket.\n");
			exit(EXIT_FAILURE);
		} else {
			switch (task->response->data[task->offset]) {
				case '\r': case '\n':
					task->flag++;
					break;
				default:
					task->flag = FLAG_RESPONSE_START;
					break;
			}
			if (task->flag == FLAG_RESPONSE_STOP) { // response got
				task->todo = save_data;
				return 0;
			} else {
				task->offset++;
				task->remain--;
				if (task->remain == 0) {
					// FIXME
					// length of response header
					fprintf(stderr, "[ao] response header too long.\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}


// 0 => finished
// 1 => EAGAIN
// 2 => get some data
int save_data(task_t *task) {
	ssize_t size = recv(task->socket_fd,
			task->response->data, task->remain, 0);
	if (size == -1) { // EAGAIN
		return 1;
	} else if (size == 0) { // finish
		task->flag = FLAG_DOWNLOAD_FINISHED;
		return 0;
	} else {
		// TODO
		// write to file
		task->start += size;
		return 2;
	}
}


///////////////////////////////////////////////////////////////////////////////


// create nonblocking tcp connection
void create_connection(task_t *task) {
	struct addrinfo hints, *res, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	Getaddrinfo(task->url->host, task->url->port, &hints, &res);

	for (rp = res; rp != NULL; rp = rp->ai_next) {
		task->socket_fd = socket(rp->ai_family,
				rp->ai_socktype | SOCK_NONBLOCK, rp->ai_protocol);
		if (task->socket_fd != -1) break; // create socket succeed
		perror("socket error");
	}
	if (rp == NULL) {
		fprintf(stderr, "[ao] can not create socket.\n");
		exit(EXIT_FAILURE);
	}

	Connect(task->socket_fd, rp->ai_addr, rp->ai_addrlen);

	freeaddrinfo(res);
}
