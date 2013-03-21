#include "ao.h"


ssize_t _send(int sockfd, void *buf, size_t len) {
	ssize_t s;
	while (1) {
		s = send(sockfd, buf, len, 0);
		if (s != -1) {
			return s;
		} else if (errno != EAGAIN) {
			perror("send error: ");
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
			perror("recv error: ");
			exit(EXIT_FAILURE);
		}
	}
}



void create_tcp_conn(ao_t *ao) {
	struct addrinfo hints, *res, *rp;
	int status;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(ao->url->host, ao->url->port, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE); // cann't get host
	}

	for (rp = res; rp != NULL; rp = rp->ai_next) {
		ao->sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (ao->sockfd == -1) {
			perror("socket error: ");
			continue;
		}

		status = connect(ao->sockfd, rp->ai_addr, rp->ai_addrlen);
		if (status == 0) {
			break; // connect succeed
		} else {
			perror("connect error: ");
			close(ao->sockfd);
		}
	}

	if (rp == NULL) {
		fprintf(stderr, "fail to create tcp connection.\n");
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res);
}



void send_all(int sockfd, void *data, size_t len) {
	ssize_t sent;
	int offset = 0;
	while (len) {
		sent = _send(sockfd, data + offset, len);
		len -= sent;
		offset += sent;
	}
}
