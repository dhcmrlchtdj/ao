int Open(const char *pathname, int flags, mode_t mode);
int Close(int fd);
FILE *Fopen(const char *path, const char *mode);
int Fclose(FILE *fp);

int Getsockopt(int sockfd, int level, int optname,
		void *optval, socklen_t *optlen);
int Getaddrinfo(const char *node, const char *service,
		const struct addrinfo *hints, struct addrinfo **res);
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t Recv(int sockfd, void *buf, size_t len);
ssize_t Send(int sockfd, const void *buf, size_t len);

int Epoll_create1(int flags);
int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int Epoll_wait(int epfd, struct epoll_event *events,
		int maxevents, int timeout);

int Timerfd_create(int clockid, int flags);

void *Malloc(size_t size);

int Gettimeofday(struct timeval *tv, struct timezone *tz);
