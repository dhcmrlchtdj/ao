#include "ao.h"

void start_download(AO *ao) {
	int taskid;
	bool support_range;
	char *p;

	// connect to url
	taskid = new_Task(ao, true, 0, 1);
	conn_url(ao->tasks[taskid], ao->url);
	printf("[ao] connect to %s\n", ao->url);

	// get filename
	get_filename_by_path(ao, ao->tasks[taskid]->url->path);
	printf("[ao] filename: %s\n", ao->filename);

	// whether support range & get filesize
	p = get_header_field(ao->tasks[taskid]->response->hf, "Content-Range");
	if (p == NULL) {
		support_range = false;
		// not support
		printf("[ao] ! not support 'Range'.\n");
		p = get_header_field(ao->tasks[taskid]->response->hf,
				"Content-Length");
		if (p == NULL) {
			ao->filesize = 0;
			printf("[ao] ! filesize not found.\n");
		} else {
			ao->filesize = strtoul(p, NULL, 0);
			printf("[ao] filesize: %lu\n", ao->filesize);
		}
	} else {
		support_range = true;
		get_filesize_by_range(ao, p);
		printf("[ao] filesize: %lu\n", ao->filesize);
	}

	close(ao->tasks[taskid]->sockfd);
	del_Task(ao, ao->tasks[taskid]);

	if (support_range) {
		save_multithread(ao);
	} else {
		save(ao);
	}
}
