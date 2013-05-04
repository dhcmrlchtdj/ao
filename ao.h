#ifndef _ao_h
#define _ao_h

//////////////////////////////

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <stdarg.h>
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


//////////////////////////////

#define SHORT_STR 1024
#define LONG_STR 4096
#define RECV_LEN 8192
#define MAX_REDIRECTION 5
//#define THREAD_NUM 6

typedef struct environ_t environ_t;
typedef struct header_field_t header_field_t;
typedef struct request_t request_t;
typedef struct response_t response_t;
typedef struct url_t url_t;

//////////////////////////////

#include "common.h"
#include "http_header.h"
#include "http_request.h"
#include "http_response.h"
#include "text.h"
#include "url.h"

#endif
