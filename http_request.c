#include "ao.h"


request_t *init_request_t(void) {
	request_t *req = malloc(sizeof(request_t));
	req->request = NULL;
	req->hf = NULL;
	return req;
}



void free_request_t(request_t *request) {
	free_header_field_t(request->hf);
	/*free(request->request);*/
	free(request);
}



void gen_basic_request_header(ao_t *ao) {
	char host[1024];
	sprintf(host, "%s:%s", ao->url->host, ao->url->port);
	add_header_field(ao->request->hf, "Host", host);
	add_header_field(ao->request->hf, "Connection", "close");
	add_header_field(ao->request->hf, "Accept-Encoding", "identity");
	add_header_field(ao->request->hf, "User-Agent", "ao/pre-alpha");
}



void send_request(ao_t *ao) {
	char *format = "GET %s HTTP/1.0\r\n%s\r\n";
	char *req = header_to_string(ao->request->hf);
	char buff[4096];
	sprintf(buff, "GET %s HTTP/1.0\r\n%s\r\n", ao->url->path, req);
	free(req);

	send_all(ao->sockfd, buff, strlen(buff));
}
