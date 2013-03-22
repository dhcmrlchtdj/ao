#include "ao.h"

#define DEFAULT_TASKLET 6


void direct_download(char *url) {
	int status, multi;
	char *ptr;

	// initial
	ao_t *ao = init_ao_t(DEFAULT_TASKLET);
	memcpy(ao->url, url, strlen(url) + 1);
	ao->tasklets[0] = init_tasklet_t(0, 0);

	// connect
	conn_url(ao->tasklets[0], url);

	// get filename
	get_filename_by_path(ao);
	printf("[ao] filename: %s\n", ao->filename);

	// whether support range
	ptr = get_header_field(ao->tasklets[0]->response->hf, "Content-Range");
	if (ptr == NULL) {
		fprintf(stdout, "[ao] not support Range header.\n");
		// get filesize
		ptr = get_header_field(ao->tasklets[0]->response->hf, "Content-Length");
		if (ptr == NULL) {
			fprintf(stderr, "[ao] cann't get filesize\n");
		} else {
			ao->filesize = strtoul(ptr, NULL, 0);
			printf("[ao] filesize: %lu\n", ao->filesize);
		}
		multi = 0;
	} else {
		get_filesize_by_range(ao, ptr);
		printf("[ao] filesize: %lu\n", ao->filesize);
		multi = 1;
	}

	close(ao->tasklets[0]->sockfd);
	free_tasklet_t(ao->tasklets[0]);

	// download
	save(ao, multi);

	free_ao_t(ao);
}
