int tcp_conn(url_t *url);
void send_all(int sockfd, char *data, size_t len);

ssize_t _send(int sockfd, void *buf, size_t len);
ssize_t _recv(int sockfd, void *buf, size_t len);
