int Open(const char *pathname, int flags, mode_t mode);
void Close(int fd);
void Pwrite(int fd, const char *buf, size_t count, off_t offset);
FILE *Fopen(const char *path, const char *mode);
void Fclose(FILE *fp);
void Unlink(const char *pathname);

void Getsockopt(int sockfd, int level, int optname,
		void *optval, socklen_t *optlen);
void Getaddrinfo(const char *node, const char *service,
		const struct addrinfo *hints, struct addrinfo **res);
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t Recv(int sockfd, void *buf, size_t len);
ssize_t Send(int sockfd, const void *buf, size_t len);
void Shutdown(int sockfd, int how);

int Epoll_create(void);
void Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int Epoll_wait(int epfd, struct epoll_event *events,
		int maxevents, int timeout);

void *Malloc(size_t size);
void *Calloc(size_t nmemb, size_t size);

void Gettimeofday(struct timeval *tv);
int Timerfd_create(void);
int Signalfd(const sigset_t *mask);
void Sigemptyset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
void Sigprocmask(int how, const sigset_t *set);
