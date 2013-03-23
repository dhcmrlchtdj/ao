void create_tcp_conn(Task *task);

void send_all(int sockfd, void *data, size_t len);

ssize_t _send(int sockfd, void *buf, size_t len);
ssize_t _recv(int sockfd, void *buf, size_t len);

void nonblocking(int fd);

void save(AO *ao);
