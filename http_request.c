#include "ao.h"

request_t *init_request(void) {
	request_t *request = malloc(sizeof(request_t));
	request->hf = NULL;
	return request;
}



void free_request(request_t *request) {
	free_header_field(request->hf);
	free(request);
}



void gen_basic_request_header(task_t *task) {
	header_field_t *ptr;
	char *buff = malloc(sizeof(char) * SHORT_STR);

	sprintf(buff, "%s:%s", task->url->host, task->url->port);
	task->request->hf = init_header_field("Host", buff);

	ptr = task->request->hf;
	ptr->next = init_header_field("Connection", "close");

	ptr = ptr->next;
	ptr->next = init_header_field("User-Agent", "ao/pre-alpha");

	if (task->add_range) {
		sprintf(buff, "bytes=%ld-%ld", task->range_start, task->range_stop);
		ptr = ptr->next;
		ptr->next = init_header_field("Range", buff);
	}

	free(buff);
}



void send_request(task_t *task) {
	char *hf_string = malloc(sizeof(char) * LONG_STR);
	memset(hf_string, 0, sizeof(char) * LONG_STR);
	header_field_t *hf = task->request->hf;
	while (hf) {
		strcat(hf_string, hf->name);
		strcat(hf_string, ": ");
		strcat(hf_string, hf->value);
		strcat(hf_string, "\r\n");
		hf = hf->next;
	}

	sprintf(task->request->string, "GET %s HTTP/1.0\r\n%s\r\n",
			task->url->path, hf_string);
	send_all(task->sockfd, task->request->string,
			strlen(task->request->string));
	free(hf_string);
}
