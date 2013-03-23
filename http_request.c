#include "ao.h"

Request *init_Request(void) {
	Request *request = malloc(sizeof(Request));
	request->hf = NULL;
	return request;
}



void free_Request(Request *request) {
	free_HeaderField(request->hf);
	/*free(request->request);*/
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
	ptr->next = init_HeaderField("Accept-Encoding", "identity");

	ptr = ptr->next;
	ptr->next = init_HeaderField("User-Agent", "ao/pre-alpha");

	if (task->range_type == 1) {
		sprintf(buff, "bytes=%lu-", task->start);
		ptr = ptr->next;
		ptr->next = init_HeaderField("Range", buff);
	} else if (task->range_type == 2) {
		sprintf(buff, "bytes=%lu-%lu", task->start, task->stop);
		ptr = ptr->next;
		ptr->next = init_HeaderField("Range", buff);
	}

	free(buff);
}



void send_request(Task *task) {
	char *format = "GET %s HTTP/1.0\r\n%s\r\n";
	char *req = header_to_string(task->request->hf);
	char *buff = malloc(sizeof(char) * LONG_STR);
	sprintf(buff, "GET %s HTTP/1.0\r\n%s\r\n", task->url->path, req);

	send_all(task->sockfd, buff, strlen(buff));
	free(req);
	free(buff);
}
