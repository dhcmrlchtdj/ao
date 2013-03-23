#ifndef _ao_h
#define _ao_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

///////////////////

typedef struct AO AO;
typedef struct Task Task;
typedef struct Url Url;
typedef struct Request Request;
typedef struct Response Response;
typedef struct Header Header;

///////////////////

#include "download.h"
#include "http_header.h"
#include "http_request.h"
#include "http_response.h"
#include "http_util.h"
#include "option.h"
#include "transport.h"

///////////////////

struct Task {
	int sockfd;
	int range_type;
	unsigned long start;
	unsigned long stop;
	Url *url;
	Request *request;
	Response *response;
};

int new_task(AO *ao, int type, unsigned long start, unsigned long stop);
Task *init_task(int type, unsigned long start, unsigned long stop);
void clear_task(Task *task);
void free_task(Task *task);


struct AO {
	bool use_task;
	int max_task;
	int used_task;
	char url[1024];
	char filename[1024];
	unsigned long filesize;
	FILE *file;
	Task *tasks[]; // array of pointer to struct task
};

AO *init_ao(int num);
void free_ao(AO *ao);



char *copy_str(char *src, size_t len);

#endif
