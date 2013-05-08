#include "ao.h"


int Getsockopt(int sockfd, int level, int optname,
		void *optval, socklen_t *optlen) {
	int s = getsockopt(sockfd, level, optname, optval, optlen);
	if (s == -1) {
		perror("getsockopt error");
		exit(EXIT_FAILURE);
	}
	return s;
}



ssize_t Send(int sockfd, const void *buf, size_t len) {
	ssize_t s = send(sockfd, buf, len, MSG_DONTWAIT);
	if (s == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		perror("send error");
		exit(EXIT_FAILURE);
	}
	return s;
}



ssize_t Recv(int sockfd, void *buf, size_t len) {
	ssize_t s = recv(sockfd, buf, len, MSG_DONTWAIT);
	if (s == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		perror("recv error");
		exit(EXIT_FAILURE);
	}
	return s;
}



int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int s = connect(sockfd, addr, addrlen);
	if (s == -1 && errno != EINPROGRESS) {
		perror("connect error");
		exit(EXIT_FAILURE);
	}
	return s;
}




int Getaddrinfo(const char *node, const char *service,
		const struct addrinfo *hints,
		struct addrinfo **res) {
	int s = getaddrinfo(node, service, hints, res);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	return s;
}



int Open(const char *pathname, int flags, mode_t mode) {
	int s = open(pathname, flags, mode);
	if (s == -1) {
		perror("open error");
		exit(EXIT_FAILURE);
	}
	return s;
}



int Close(int fd) {
	int s = close(fd);
	if (s == -1) {
		perror("close error");
		exit(EXIT_FAILURE);
	}
	return s;
}



FILE *Fopen(const char *path, const char *mode) {
	FILE *s = fopen(path, mode);
	if (s == NULL) {
		perror("fopen error");
		exit(EXIT_FAILURE);
	}
	return s;
}



int Fclose(FILE *fp) {
	int s = fclose(fp);
	if (s == EOF) {
		perror("fclose error");
		exit(EXIT_FAILURE);
	}
	return s;
}


int Epoll_wait(int epfd, struct epoll_event *events,
		int maxevents, int timeout) {
	int s = Epoll_wait(epfd, events, maxevents, timeout);
	if (s == -1) {
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	return s;
}



int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
	int s = epoll_ctl(epfd, op, fd, event);
	if (s == -1) {
		perror("epoll_ctl error");
		exit(EXIT_FAILURE);
	}
	return s;
}



int Epoll_create1(int flags) {
	int s = Epoll_create1(flags);
	if (s == -1) {
		perror("epoll_create1 error");
		exit(EXIT_FAILURE);
	}
	return s;
}



int Timerfd_create(int clockid, int flags) {
	int s = timerfd_create(clockid, flags);
	if (s == -1) {
		perror("timerfd_create error");
		exit(EXIT_FAILURE);
	}
	return s;
}



void *Malloc(size_t size) {
	void *s = malloc(size);
	if (s == NULL && size != 0) {
		fprintf(stderr, "malloc error.\n");
		exit(EXIT_FAILURE);
	}
	return s;
}



int Gettimeofday(struct timeval *tv, struct timezone *tz) {
	int s = gettimeofday(tv, tz);
	if (s == -1) {
		perror("gettimeofday error");
		exit(EXIT_FAILURE);
	}
	return s;
}
