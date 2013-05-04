//dest_size contains '\0'
//src_size not contains '\0'
void static_copy(char *dest, size_t dest_size, char *src, size_t src_size);
char *dynamic_copy(char *src, size_t src_size);

//////////////////////////////

struct environ_t {
	int file_fd;
	int epoll_fd;
	int timer_fd;
	//int signal_fd;
	int partition; // partition count
	char filename[SHORT_STR]; // filename
	url_t *url; // download url
	task_t *tasks; // task_t array
};

environ_t *new_environ(void);
void del_environ(environ_t *env);
void environ_update(environ_t* env);

//////////////////////////////

struct task_t {
	int socket_fd;
	off_t current;
	off_t offset;
	off_t remain; // remain to send or space to store
	char request[LONG_STR];
	char *response;
};

void initial_task(task_t *task, request_t *request,
		off_t start_pos, off_t stop_pos);
void destory_task(task_t *task);

//////////////////////////////

void set_alarm(int timer_fd, long nsec);
