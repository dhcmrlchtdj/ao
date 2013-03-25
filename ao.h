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
#include <time.h>

///////////////////

#define SHORT_STR 1024
#define LONG_STR 4096
#define RECV_LEN 8192
#define MAX_REDIRECTION 5

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
	int fd; // file descripter
	off_t filesize;
	int task_num; // max threads
	char filename[SHORT_STR];
	char url[SHORT_STR];
	bool support_range; // whether support range header
	pthread_t main_thread;
	pthread_mutex_t mutex;
};

env_t *init_env(void);
void free_env(env_t *env);



struct task_t {
	int sockfd;
	env_t *env;
	url_t *url;
	request_t *request;
	response_t *response;
	bool add_range;
	off_t range_start;
	off_t range_stop;
};

// ... == off_t start, off_t stop
task_t *init_task(env_t *env, bool add_range, ...);
void free_task(task_t *task);
void clear_task(task_t *task);

//////////

//dest_size contains '\0'
//src_size not contains '\0'
char *dynamic_copy(char *src, size_t src_size);
void static_copy(char *dest, size_t dest_size, char *src, size_t src_size);

#endif
