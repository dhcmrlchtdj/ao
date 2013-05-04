//dest_size contains '\0'
//src_size not contains '\0'
void static_copy(char *dest, size_t dest_size, char *src, size_t src_size);
char *dynamic_copy(char *src, size_t src_size);


struct environ {
	int epoll_fd;
	int timer_fd;
	int signal_fd;
	int partition; // partition count
	char filename[SHORT_STR]; // filename
	url_t *url; // download url
};
environ_t *new_environ(void);
void del_environ(environ_t *env);
void environ_update(environ_t* env);


void p(environ_t *e);



void set_alarm(int timer_fd, long nsec);
