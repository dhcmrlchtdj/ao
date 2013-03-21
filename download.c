#include "ao.h"


void direct_download(char *url) {
	ao_t *ao = init_ao_t();
	int status;
	char *val;

	status = conn_url(ao, url);
	if (status == -1) {
		fprintf(stderr, "direct download error\n");
		exit(EXIT_FAILURE);
	}

	val = 
}
