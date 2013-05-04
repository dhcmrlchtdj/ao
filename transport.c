#include "ao.h"


ssize_t _send(int sockfd, void *buf, size_t len) {
	ssize_t s = send(sockfd, buf, len, 0);
	if (s != -1) {
		return s;
	} else {
		perror("send error");
		return 0;
	}
}



ssize_t _recv(int sockfd, void *buf, size_t len) {
	ssize_t s = recv(sockfd, buf, len, 0);
	if (s != -1) {
		return s;
	} else {
		perror("send error");
		return 0;
	}
}



void tcp_conn(task_t *task) {
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
