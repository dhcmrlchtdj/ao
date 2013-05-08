#include "ao.h"


request_t *new_request(void) {
	request_t *req = Malloc(sizeof(request_t));
	req->hf = NULL;
	req->url = NULL;
	return req;
}



void del_request(request_t *req) {
	del_header_field(req->hf);
	free(req);
}



void gen_request_header(request_t *req) {
	header_field_t *ptr;
	char *hf_value = Malloc(sizeof(char) * SHORT_STR);
	// add Host
	if (strcmp(req->url->port, "80") == 0) {
		req->hf = new_header_field("Host", req->url->host);
	} else {
		snprintf(hf_value, SHORT_STR, "%s:%s",
			   	req->url->host, req->url->port);
		req->hf = new_header_field("Host", hf_value);
	}
	// add Connection
	ptr = req->hf;
	ptr->next = new_header_field("Connection", "close");
	// add user-agent
	ptr = ptr->next;
	ptr->next = new_header_field("User-Agent", "ao/pre-alpha");

	free(hf_value);
}



// convert request_t to string
void request2string(request_t *req) {
	char *hf_string = Malloc(LONG_STR * sizeof(char));
	header_field_t *hf = req->hf;
	while (hf) {
		strcat(hf_string, hf->name);
		strcat(hf_string, ": ");
		strcat(hf_string, hf->value);
		strcat(hf_string, "\r\n");
		hf = hf->next;
	}

	snprintf(req->data, LONG_STR, "GET %s HTTP/1.0\r\n%s\r\n",
			req->url->path, hf_string);

	free(hf_string);
}



void set_range(request_t *req, off_t start, off_t stop) {
	char value[SHORT_STR];
	snprintf(value, SHORT_STR, "bytes=%ld-%ld", start, stop);
	set_header(req->hf, "Range", value);
}
