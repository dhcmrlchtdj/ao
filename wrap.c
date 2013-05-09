#include "ao.h"


void Getsockopt(int sockfd, int level, int optname,
		void *optval, socklen_t *optlen) {
	if (getsockopt(sockfd, level, optname, optval, optlen) == -1) {
		perror("getsockopt error");
		exit(EXIT_FAILURE);
	}
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



void Shutdown(int sockfd, int how) {
	if (shutdown(sockfd, how) == -1) {
		perror("shutdown error");
		exit(EXIT_FAILURE);
	}
}



void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int s = connect(sockfd, addr, addrlen);
	if (s == -1 && errno != EINPROGRESS) {
		perror("connect error");
		exit(EXIT_FAILURE);
	}
}




void Getaddrinfo(const char *node, const char *service,
		const struct addrinfo *hints, struct addrinfo **res) {
	int s = getaddrinfo(node, service, hints, res);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
}



int Open(const char *pathname, int flags, mode_t mode) {
	int s = open(pathname, flags, mode);
	if (s == -1) {
		perror("open error");
		exit(EXIT_FAILURE);
	}
	return s;
}



void Close(int fd) {
	if (close(fd) == -1) {
		perror("close error");
		exit(EXIT_FAILURE);
	}
}



void Pwrite(int fd, const char *buf, size_t count, off_t offset) {
	ssize_t s;
	while (1) {
		s = pwrite(fd, buf, count, offset);
		if (s == -1) {
			perror("pwrite error");
			exit(EXIT_FAILURE);
		}
		if (s == count) return;
		count -= s;
		offset += s;
		buf += s;
	}
}



FILE *Fopen(const char *path, const char *mode) {
	FILE *s = fopen(path, mode);
	if (s == NULL) {
		perror("fopen error");
		exit(EXIT_FAILURE);
	}
	return s;
}



void Fclose(FILE *fp) {
	if (fclose(fp) == EOF) {
		perror("fclose error");
		exit(EXIT_FAILURE);
	}
}



void Unlink(const char *pathname) {
	if (unlink(pathname) == -1) {
		perror("unlink error");
		exit(EXIT_FAILURE);
	}
}



int Epoll_wait(int epfd, struct epoll_event *events,
		int maxevents, int timeout) {
	int s = epoll_wait(epfd, events, maxevents, timeout);
	if (s == -1) {
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	return s;
}



void Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
	if (epoll_ctl(epfd, op, fd, event) == -1) {
		perror("epoll_ctl error");
		exit(EXIT_FAILURE);
	}
}



int Epoll_create(void) {
	int s = epoll_create1(0);
	if (s == -1) {
		perror("epoll_create1 error");
		exit(EXIT_FAILURE);
	}
	return s;
}



int Timerfd_create(void) {
	int s = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
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



void *Calloc(size_t nmemb, size_t size) {
	void *s = calloc(nmemb, size);
	if (s == NULL && nmemb != 0 && size != 0) {
		fprintf(stderr, "calloc error.\n");
		exit(EXIT_FAILURE);
	}
	return s;
}



void Gettimeofday(struct timeval *tv) {
	if (gettimeofday(tv, NULL) == -1) {
		perror("gettimeofday error");
		exit(EXIT_FAILURE);
	}
}



int Signalfd(const sigset_t *mask) {
	int s = signalfd(-1, mask, SFD_NONBLOCK);
	if (s == -1) {
		perror("signalfd error");
		exit(EXIT_FAILURE);
	}
	return s;
}



void Sigemptyset(sigset_t *set) {
	if (sigemptyset(set) == -1) {
		fprintf(stderr, "sigemptyset error.\n");
		exit(EXIT_FAILURE);
	}
}



void Sigaddset(sigset_t *set, int signum) {
	if (sigaddset(set, signum) == -1) {
		fprintf(stderr, "sigaddset error.\n");
		exit(EXIT_FAILURE);
	}
}



void Sigprocmask(int how, const sigset_t *set) {
	if (sigprocmask(how, set, NULL) == -1) {
		fprintf(stderr, "sigprocmask error\n");
		exit(EXIT_FAILURE);
	}
}
