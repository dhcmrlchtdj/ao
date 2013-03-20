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
#include "http.h"
#include "option.h"
#include "transport.h"

///////////////////

struct ao_t {
	int sockfd;
	char *filename;
	FILE *file;
	url_t *url;
	request_t *request;
	response_t *response;
};

void free_ao_t(ao_t *ao);
char *_copy_str(char *src, size_t len);


#endif
