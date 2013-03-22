#ifndef _ao_h
#define _ao_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

///////////////////

typedef struct ao_t ao_t;
typedef struct tasklet_t tasklet_t;
typedef struct url_t url_t;
typedef struct request_t request_t;
typedef struct response_t response_t;
typedef struct header_field_t header_field_t;

///////////////////

#include "download.h"
#include "http_header.h"
#include "http_request.h"
#include "http_response.h"
#include "http_util.h"
#include "option.h"
#include "transport.h"

///////////////////

struct tasklet_t {
	int sockfd;
	unsigned long received;
	unsigned long start;
	unsigned long stop;
	url_t *url;
	request_t *request;
	response_t *response;
};

tasklet_t *init_tasklet_t(unsigned long start, unsigned long stop);
void clear_tasklet_t(tasklet_t *tasklet);
void free_tasklet_t(tasklet_t *tasklet);


struct ao_t {
	char url[1024];
	char filename[1024];
	unsigned long filesize;
	FILE *file;
	int task_count;
	tasklet_t *tasklets[]; // array of pointer to tasklet_t
};

ao_t *init_ao_t(int num);
void free_ao_t(ao_t *ao);



char *copy_str(char *src, size_t len);

#endif
