#include "ao.h"


ssize_t _send(int sockfd, void *buf, size_t len) {
	ssize_t s;
	while (1) {
		s = send(sockfd, buf, len, 0);
		if (s != -1) {
			return s;
		} else if (errno != EAGAIN) {
			perror("send error");
			exit(EXIT_FAILURE);
		}
	}
}



ssize_t _recv(int sockfd, void *buf, size_t len) {
	ssize_t s;
	while (1) {
		s = recv(sockfd, buf, len, 0);
		if (s != -1) {
			return s;
		} else if (errno != EAGAIN) {
			perror("recv error");
			exit(EXIT_FAILURE);
		}
	}
}



void create_tcp_conn(Task *task) {
	struct addrinfo hints, *res, *rp;
	int status, sockfd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(task->url->host, task->url->port, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		printf("%s, %s\n", task->url->host, task->url->port);
		exit(EXIT_FAILURE); // cann't get host
	}

	for (rp = res; rp != NULL; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1) {
			perror("socket error");
			continue;
		}

		status = connect(sockfd, rp->ai_addr, rp->ai_addrlen);
		if (status == 0) {
			break; // connect succeed
		} else {
			perror("connect error");
			close(sockfd);
		}
	}

	if (rp == NULL) {
		fprintf(stderr, "fail to create tcp connection.\n");
		exit(EXIT_FAILURE);
	} else {
		task->sockfd = sockfd;
	}

	freeaddrinfo(res);
}



void send_all(int sockfd, void *data, size_t len) {
	ssize_t sent;
	long offset = 0;
	while (len) {
		sent = _send(sockfd, data + offset, len);
		len -= sent;
		offset += sent;
	}
}



/*
void nonblocking(int fd) {
	int flags, status;
	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		perror("nonblocking get error");
		exit(EXIT_FAILURE);
	}
	status = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	if (status == -1) {
		perror("nonblocking set error");
		exit(EXIT_FAILURE);
	}
}
*/


void save(AO *ao) {
	char buff[RECV_LEN];
	ssize_t received;
	int taskid;
	Task *task;
	printf("[ao] start download\n");

	ao->file = fopen(ao->filename, "w+b");
	taskid = new_Task(ao, false, 0, 0);
	task = ao->tasks[taskid];
	conn_url(task, ao->url);
	while (1) {
		received = _recv(task->sockfd, buff, RECV_LEN);
		if (received == 0) break;
		fwrite(buff, sizeof(char), received, ao->file);
		task->received += received;
		printf("%ld ", received);
	}

	printf("\n[ao] download finish\n");
	fclose(ao->file);
	close(task->sockfd);
	del_Task(ao, task);
}



void save_multithread(AO *ao) {
	printf("[ao] start download\n");

	char buff[RECV_LEN];
	ssize_t received;
	setup_Tasks(ao);

	/*ao->file = fopen(ao->filename, "w+b");*/
	/*fclose(ao->file);*/
}

