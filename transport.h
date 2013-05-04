ssize_t _send(int sockfd, void *buf, size_t len);
ssize_t _recv(int sockfd, void *buf, size_t len);

void tcp_conn(task_t *task);
