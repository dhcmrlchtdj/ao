#ifndef _ao_h
#define _ao_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
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

struct ao_t {
	char *filename;
	FILE *file;
	unsigned long size;
	tasklet_t tasklets[];
};

char *copy_str(char *src, size_t len);

#endif
