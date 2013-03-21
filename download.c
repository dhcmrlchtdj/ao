#include "ao.h"


void direct_download(char *url) {
	int num = 6;
	ao_t *ao = malloc(sizeof(ao_t) + num * sizeof(tasklet_t));

	int status;
	char *val;

	status = conn_url(&ao->tasklets[0], url);
	if (status == -1) {
		fprintf(stderr, "direct download error\n");
		exit(EXIT_FAILURE);
	}

	printf("status: %s\n", ao->tasklets[0].response->status);

	char *p = get_header_field(ao->tasklets[0].response->hf, "Content-Length");
	printf("%s\n", p);

	/*ao->size = strtoul(get_header_field(ao->tasklets[0].response*/

}
