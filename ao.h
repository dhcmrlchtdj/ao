#ifndef _ao_h
#define _ao_h

///////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
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
	int partition; // partition count
	char filename[SHORT_STR]; // filename
	url_t *url; // download url
	task_t *tasks; // task_t array
};

void initial_environ(environ_t *env);
void destroy_environ(environ_t *env);


struct task_t {
	int socket_fd;

	void (*todo)(task_t *task);

	url_t *url;
	request_t *request;
	response_t *response;

	int stop_flag;
	// stop_flag == 0,1,2,3 => filtering response
	// stop_flag == 4 => filter finish
	// stop_flag == 5 => download finish
	size_t remain;
	off_t offset;
	off_t current; // offset for pwrite
};

void initial_task(task_t *task, off_t start_pos, off_t stop_pos);
//void initial_task(task_t *task, request_t *request, off_t start_pos, off_t stop_pos);
void destroy_task(task_t *task);

#endif
