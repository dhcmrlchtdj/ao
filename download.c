#include "ao.h"


void start_download(AO *ao) {
	int status, taskid;
	char *ptr;

	// initial task
	taskid = new_task(ao, 2, 0, 0);

	// connect
	conn_url(ao->tasks[taskid], ao->url);
	printf("[ao] connect to %s\n", ao->url);

	// get filename
	get_filename_by_path(ao, ao->tasks[taskid]->url->path);
	printf("[ao] filename: %s\n", ao->filename);

	// whether support range
	ptr = get_header(ao->tasks[taskid]->response->hf, "Content-Range");
	if (ptr == NULL) {
		ao->use_task = false;
		printf("[ao] ! not support 'Range'.\n");
		// get filesize
		ptr = get_header(ao->tasks[taskid]->response->hf, "Content-Length");
		if (ptr == NULL) {
			printf("[ao] ! 'Content-Length' not found.\n");
		} else {
			ao->filesize = strtoul(ptr, NULL, 0);
			printf("[ao] filesize: %lu\n", ao->filesize);
		}
	} else {
		ao->use_task = true;
		get_filesize_by_range(ao, ptr);
		printf("[ao] filesize: %lu\n", ao->filesize);
	}

	close(ao->tasks[taskid]->sockfd);
	free_task(ao->tasks[taskid]);

	// download
	save(ao);

	free_ao(ao);
}
