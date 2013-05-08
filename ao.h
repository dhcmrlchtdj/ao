#ifndef _ao_h
#define _ao_h

///////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/signalfd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <unistd.h>

///////////////////////////////////////////////////////////////////////////////

#define SHORT_STR 1024
#define LONG_STR 4096
#define RECV_SIZE 8192
#define MAX_REDIRECTION 5
#define DEFAULT_PARTITION 6

#define FLAG_RESPONSE_START 0
#define FLAG_RESPONSE_STOP 4

typedef struct environ_t environ_t;
typedef struct header_field_t header_field_t;
typedef struct request_t request_t;
typedef struct response_t response_t;
typedef struct url_t url_t;
typedef struct task_t task_t;
typedef struct data_t data_t;

///////////////////////////////////////////////////////////////////////////////

#include "download.h"
#include "http_header.h"
#include "http_request.h"
#include "http_response.h"
#include "text.h"
#include "transport.h"
#include "url.h"
#include "utils.h"

///////////////////////////////////////////////////////////////////////////////

struct environ_t {
	int file_fd;
	int epoll_fd;
	int timer_fd;
	//int signal_fd;
	struct timeval t1, t2;
	size_t last_size;
	size_t filesize;
	int partition; // partition count
	bool support_range;
	bool has_log;
	char logfile[SHORT_STR]; // log file
	char filename[SHORT_STR]; // filename
	url_t url; // download url
	task_t *tasks; // task_t array
};

void initial_environ(environ_t *env);
void destroy_environ(environ_t *env);
void environ_update_by_log(environ_t *env);



struct task_t {
	int socket_fd;
	struct epoll_event event;
	int (*todo)(task_t *task);
	int flag;
	int redirection;
	off_t start, stop;
	off_t offset;
	size_t remain;
	url_t *url;
	request_t *request;
	response_t *response;
};

task_t *new_task(off_t start, off_t stop);
void del_task(task_t *task);
void initial_task(task_t *task, off_t start, off_t stop);
void destroy_task(task_t *task);
void task_update_request(task_t *task);
void task_prepare_redirection(task_t *task);
void task_update_by_log(task_t *task);


#endif
