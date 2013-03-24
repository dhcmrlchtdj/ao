#ifndef _ao_h
#define _ao_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
//#include <fcntl.h>
//#include <sys/epoll.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <pthread.h>

///////////////////

#define SHORT_STR 1024
#define LONG_STR 4096
#define RECV_LEN 8192
#define MAX_REDIRECTION 5

typedef struct AO AO;
typedef struct Task Task;
typedef struct Url Url;
typedef struct Request Request;
typedef struct Response Response;
typedef struct HeaderField HeaderField;

///////////////////

#include "download.h"
#include "http_header.h"
#include "http_request.h"
#include "http_response.h"
#include "http_util.h"
#include "text.h"
#include "transport.h"
#include "url.h"

///////////////////

struct Task {
	int taskid;
	int sockfd;
	bool range;
	unsigned long start;
	unsigned long stop;
	unsigned long received;
	Url *url;
	Request *request;
	Response *response;
};

Task *init_Task(bool range, unsigned long start, unsigned long stop);
void clear_Task(Task *task);
void free_Task(Task *task);
int new_Task(AO *ao, bool range, unsigned long start, unsigned long stop);
void del_Task(AO *ao, Task *task);
void setup_Tasks(AO *ao);

struct AO {
	char url[SHORT_STR];
	char filename[SHORT_STR];
	unsigned long filesize;
	FILE *file;
	int max_tasks;
	Task *tasks[]; // array of pointer to struct task
};

AO *init_AO(int num);
void free_AO(AO *ao);

//////////

/* src_size not contain '\0' */
char *dynamic_copy(char *src, size_t src_size);
void static_copy(char *dest, size_t dest_size, char *src, size_t src_size);

#endif
