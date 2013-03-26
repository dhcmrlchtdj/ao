#ifndef _ao_h
#define _ao_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>

///////////////////

#define SHORT_STR 1024
#define LONG_STR 4096
#define RECV_LEN 8192
#define MAX_REDIRECTION 5
#define THREAD_NUM 6

typedef struct env_t env_t;
typedef struct header_field_t header_field_t;
typedef struct request_t request_t;
typedef struct response_t response_t;
typedef struct task_t task_t;
typedef struct url_t url_t;

#include "download.h"
#include "http_header.h"
#include "http_request.h"
#include "http_response.h"
#include "http_util.h"
#include "text.h"
#include "transport.h"
#include "url.h"

///////////////////

struct env_t {
	bool support_range; // whether support range header
	short task_num; // threads num
	int fd; // file descripter
	off_t filesize;
	off_t last_size; // used to calculate speed
	struct timeval t1, t2; // used to calculate speed
	pthread_mutex_t mutex; // only one thread is write progress bar
	char filename[SHORT_STR];
	char url[SHORT_STR];
};

void init_env(env_t *env);
void free_env(env_t *env);



struct task_t {
	bool add_range;
	int sockfd;
	off_t range_start;
	off_t range_stop;
	url_t *url;
	request_t *request;
	response_t *response;
};

// ... == off_t start, off_t stop
task_t *init_task(bool add_range, ...);
void free_task(task_t *task);
void clear_task(task_t *task);

//////////

//dest_size contains '\0'
//src_size not contains '\0'
char *dynamic_copy(char *src, size_t src_size);
void static_copy(char *dest, size_t dest_size, char *src, size_t src_size);

// return ms. t2 must greater than t1.
long delta_time(struct timeval *t1, struct timeval *t2);

//////////

env_t env;

#endif
