#include "ao.h"


void wait_connect(task_t *task) {
	int status, value;
	socklen_t len = sizeof(value);
	status = getsockopt(task->socket_fd, SOL_SOCKET, SO_ERROR, &value, &len);
	if (status == -1) {
		perror("getsockopt error");
		exit(EXIT_FAILURE);
	}
	if (value == 0) {
		// connect succeed
		task->todo = send_request;
		task->offset = 0;
		task->remain = strlen(task->request->data);
	}
}



void send_request(task_t *task) {
	ssize_t size = send(task->socket_fd,
			task->request->data + task->offset, task->remain, 0);
	if (size == -1) { // error
		if (errno != EAGAIN) {
			perror("send error");
			exit(EXIT_FAILURE);
		}
	} else if (size == 0) { // finish
		task->todo = recv_response;
		task->offset = 0;
		task->remain = LONG_STR;
	} else {
		task->offset += size;
		task->remain -= size;
	}
}



void recv_response(task_t *task) {
	ssize_t size;
	while (1) {
		size = recv(task->socket_fd,
				task->response->data + task->offset, 1, 0);
		if (size == -1) { // error
			if (errno == EAGAIN) {
				break;
			} else {
				perror("recv error");
				exit(EXIT_FAILURE);
			}
		} else if (size == 0) { // close before succeed
			fprintf(stderr, "[ao] some wrong on socket.\n");
			exit(EXIT_FAILURE);
		} else {
			switch (task->response->data[task->offset]) {
				case '\r': case '\n':
					task->stop_flag++;
					break;
				default:
					task->stop_flag = 0;
				break;
			}
			if (task->stop_flag == 4) { // response got
				task->todo = save_data;
				task->remain = LONG_STR;
				break;
			} else {
				task->offset++;
				task->remain--;
				if (task->remain == 0) {
					fprintf(stderr, "[ao] some wrong on socket.\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}



void save_data(task_t *task) {
	ssize_t size = recv(task->socket_fd,
			task->response->data, task->remain, 0);
	if (size == -1) { // error
		if (errno != EAGAIN) {
			perror("recv error");
			exit(EXIT_FAILURE);
		}
	} else if (size == 0) { // finish
		task->todo = NULL;
		task->stop_flag = 5;
	} else {
		// TODO
		// write to file
		task->current += size;
	}
}


///////////////////////////////////////////////////////////////////////////////


void create_connection(task_t *task) {
	int status;
	struct addrinfo hints, *res, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(task->url->host, task->url->port, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "[ao] getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	for (rp = res; rp != NULL; rp = rp->ai_next) {
		task->socket_fd = socket(rp->ai_family,
				rp->ai_socktype | SOCK_NONBLOCK, rp->ai_protocol);
		if (task->socket_fd != -1) break;
		perror("socket error");
	}
	if (rp == NULL) {
		fprintf(stderr, "[ao] can not create socket.\n");
		exit(EXIT_FAILURE);
	}

	status = connect(task->socket_fd, rp->ai_addr, rp->ai_addrlen);
	if (status == -1 && errno != EINPROGRESS) {
		perror("connect error");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res);
}
