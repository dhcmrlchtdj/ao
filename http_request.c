#include "ao.h"

request_t *init_request_t(void) {
	request_t *request = malloc(sizeof(request_t));
	request->hf = NULL;
	return request;
}



void free_request_t(request_t *request) {
	free_header_field_t(request->hf);
	/*free(request->request);*/
	free(request);
}



void gen_basic_request_header(tasklet_t *tasklet) {
	header_field_t *ptr;

	char *buff = malloc(sizeof(char) * 1024);

	sprintf(buff, "%s:%s", tasklet->url->host, tasklet->url->port);
	tasklet->request->hf = gen_header_field("Host", buff);

	sprintf(buff, "bytes=%lu-%lu", tasklet->start, tasklet->stop);
	tasklet->request->hf->next = gen_header_field("Range", buff);

	ptr = tasklet->request->hf->next;
	ptr->next = gen_header_field("Connection", "close");

	ptr = ptr->next;
	ptr->next = gen_header_field("Accept-Encoding", "identity");

	ptr = ptr->next;
	ptr->next = gen_header_field("User-Agent", "ao/pre-alpha");

	free(buff);
}



void send_request(tasklet_t *tasklet) {
	char *format = "GET %s HTTP/1.0\r\n%s\r\n";
	char *req = header_to_string(tasklet->request->hf);
	char *buff = malloc(sizeof(char) * 4096);
	sprintf(buff, "GET %s HTTP/1.0\r\n%s\r\n", tasklet->url->path, req);

	/* DEBUG */
	printf("===request start===\n");
	printf("%s\n", buff);
	printf("===request end===\n");

	
	send_all(tasklet->sockfd, buff, strlen(buff));
	free(req);
	free(buff);
}
