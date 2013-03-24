#include "ao.h"

Request *init_Request(void) {
	Request *request = malloc(sizeof(Request));
	request->hf = NULL;
	return request;
}



void free_Request(Request *request) {
	free_HeaderField(request->hf);
	free(request);
}



void gen_basic_request_header(Task *task) {
	HeaderField *ptr;
	char *buff = malloc(sizeof(char) * SHORT_STR);

	sprintf(buff, "%s:%s", task->url->host, task->url->port);
	task->request->hf = init_HeaderField("Host", buff);

	ptr = task->request->hf;
	ptr->next = init_HeaderField("Connection", "close");

	ptr = ptr->next;
	ptr->next = init_HeaderField("User-Agent", "ao/pre-alpha");

	if (task->range) {
		sprintf(buff, "bytes=%lu-%lu", task->start, task->stop);
		ptr = ptr->next;
		ptr->next = init_HeaderField("Range", buff);
	}

	free(buff);
}



void send_request(Task *task) {
	char *hf_string = malloc(sizeof(char) * LONG_STR);
	memset(hf_string, 0, sizeof(char) * LONG_STR);
	HeaderField *hf = task->request->hf;
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
