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
typedef struct header_field_t header_field_t;
typedef struct request_t request_t;
typedef struct response_t response_t;
typedef struct url_t url_t;

///////////////////

#include "download.h"
#include "http_conn.h"
#include "http_header.h"
#include "http_request.h"
#include "http_response.h"
#include "option.h"
#include "transport.h"
#include "url.h"

///////////////////

struct ao_t {
	int sockfd;
	unsigned long length;
	char *filename;
	FILE *file;
	url_t *url;
	request_t *request;
	response_t *response;
	tasklet_t *tasklet;
};

struct ao_tt {
	unsigned long file_size;
	char *file_name
	FILE *fp;
	url_t *url;
	tasklet_t *tasklets;
}

struct tasklet {
	int sockfd;
	unsigned long received;
	unsigned long start;
	unsigned long stop;
	request_t *request;
	response_t *response;
}

ao_t *init_ao_t(void);
void free_ao_t(ao_t *ao);


#endif
