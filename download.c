#include "ao.h"


void direct_download(char *url) {
	int status;
	char *val;

	int num = 6;
	ao_t *ao = init_ao_t(num);
	ao->tasklets[0] = init_tasklet_t(0, 0);

	status = conn_url(ao->tasklets[0], url);
	if (status == -1) {
		fprintf(stderr, "direct download error\n");
		exit(EXIT_FAILURE);
	}

	printf("status: %s\n", ao->tasklets[0]->response->status);

	char *p = get_header_field(ao->tasklets[0]->response->hf,
			"Content-Length");
	printf("%s\n", p);

	free_tasklet_t(ao->tasklets[0]);
	free(ao);

}
