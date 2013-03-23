#include "ao.h"

Request *init_request(void) {
	Request *request = malloc(sizeof(Request));
	request->hf = NULL;
	return request;
}



void free_request(Request *request) {
	free_header(request->hf);
	/*free(request->request);*/
	free(request);
}



void gen_basic_request_header(Task *task) {
	Header *ptr;

	char *buff = malloc(sizeof(char) * 1024);

	sprintf(buff, "%s:%s", task->url->host, task->url->port);
	task->request->hf = init_header("Host", buff);

	sprintf(buff, "bytes=%lu-%lu", task->start, task->stop);
	task->request->hf->next = init_header("Range", buff);

	ptr = task->request->hf->next;
	ptr->next = init_header("Connection", "close");

	ptr = ptr->next;
	ptr->next = init_header("Accept-Encoding", "identity");

	ptr = ptr->next;
	ptr->next = init_header("User-Agent", "ao/pre-alpha");

	free(buff);
}



void send_request(Task *task) {
	char *format = "GET %s HTTP/1.0\r\n%s\r\n";
	char *req = header_to_string(task->request->hf);
	char *buff = malloc(sizeof(char) * 4096);
	sprintf(buff, "GET %s HTTP/1.0\r\n%s\r\n", task->url->path, req);

	send_all(task->sockfd, buff, strlen(buff));
	free(req);
	free(buff);
}
