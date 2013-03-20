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



void _getaddrinfo(char *host, char *port,
		struct addrinfo *hints, struct addrinfo **res) {
	int status = getaddrinfo(host, port, hints, res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
}



int tcp_conn(url_t *url) {
	struct addrinfo hints, *res, *rp;
	int sockfd, status;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	_getaddrinfo(url->host, url->port, &hints, &res);

	for (rp = res; rp != NULL; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1) {
			perror("socket error: ");
			continue;
		}

		status = connect(sockfd, rp->ai_addr, rp->ai_addrlen);
		if (status == 0) {
			break; // connect succeed
		} else {
			perror("connect error: ");
			close(sockfd);
		}
	}
	if (rp == NULL) {
		sockfd = -1;
	}

	freeaddrinfo(res);

	return sockfd;
}



void send_all(int sockfd, char *data, size_t len) {
	ssize_t sent;
	int offset = 0;
	while (len) {
		sent = _send(sockfd, data + offset, len);
		len -= sent;
		offset += sent;
	}
}
