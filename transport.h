void create_tcp_conn(ao_t *ao);

void send_all(int sockfd, void *data, size_t len);

ssize_t _send(int sockfd, void *buf, size_t len);
ssize_t _recv(int sockfd, void *buf, size_t len);
