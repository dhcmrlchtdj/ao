#include "ao.h"


void free_request_t(request_t *request) {
	free_header_field_t(request->hf);
	/*free(request->request);*/
	free(request);
}



void gen_basic_request_header(tasklet_t *tasklet) {
	char host[1024];
	sprintf(host, "%s:%s", tasklet->url->host, tasklet->url->port);
	tasklet->request->hf = gen_header_field("Host", host);

	header_field_t *ptr = tasklet->request->hf;
	ptr->next = gen_header_field("Connection", "close");
	ptr = ptr->next;
	ptr->next = gen_header_field("Accept-Encoding", "identity");
	ptr = ptr->next;
	ptr->next = gen_header_field("User-Agent", "ao/pre-alpha");
}



void send_request(tasklet_t *tasklet) {
	char *format = "GET %s HTTP/1.0\r\n%s\r\n";
	char *req = header_to_string(tasklet->request->hf);
	char *buff = malloc(sizeof(char) * 4096);
	sprintf(buff, "GET %s HTTP/1.0\r\n%s\r\n", tasklet->url->path, req);
	send_all(tasklet->sockfd, buff, strlen(buff));
	free(req);
	free(buff);
}
