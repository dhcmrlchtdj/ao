void create_tcp_conn(tasklet_t *tasklet);

void send_all(int sockfd, void *data, size_t len);

ssize_t _send(int sockfd, void *buf, size_t len);
ssize_t _recv(int sockfd, void *buf, size_t len);

void nonblocking(int fd);


void save(ao_t *ao, int multi);
