#include "ao.h"

// not return -1
ssize_t Send(int sockfd, void *buf, size_t len) {
	ssize_t size = send(sockfd, buf, len, 0);
	if (size == -1) {
		if (errno == EAGAIN)
			return 0;
		perror("send error");
		exit(EXIT_FAILURE);
	}
	return size;
}



// return -1 only when EAGAIN
ssize_t Recv(int sockfd, void *buf, size_t len) {
	ssize_t size = recv(sockfd, buf, len, 0);
	if (size == -1 && errno != EAGAIN) {
		perror("recv error");
		exit(EXIT_FAILURE);
	}
	return size;
}



int tcp_conn(url_t *url) {
	struct addrinfo hints, *res, *rp;
	int status, sockfd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(url->host, url->port, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		printf("%s, %s\n", url->host, url->port);
		exit(EXIT_FAILURE); // cann't get host
	}

	for (rp = res; rp != NULL; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family,
				rp->ai_socktype | SOCK_NONBLOCK, rp->ai_protocol);
		if (sockfd != -1) break;
		perror("socket error");
	}
	if (rp == NULL) {
		fprintf(stderr, "fail to create tcp connection.\n");
		exit(EXIT_FAILURE);
	}

	connect(sockfd, rp->ai_addr, rp->ai_addrlen);

	freeaddrinfo(res);
	return sockfd;
}
