#include "ao.h"

request_t *new_request(url_t *url) {
	request_t *new = malloc(sizeof(request_t));
	assert(new != NULL);
	new->url = url;
	new->hf = NULL;
	return new;
}



void del_request(request_t *request) {
	del_header_field(request->hf);
	free(request);
}



// convert request_t to string
char *request2str(request_t *request) {
	size_t size = sizeof(char) * LONG_STR;

	char *hf_string = malloc(size);
	assert(hf_string != NULL);
	memset(hf_string, 0, size);

	header_field_t *hf = request->hf;
	while (hf) {
		strcat(hf_string, hf->name);
		strcat(hf_string, ": ");
		strcat(hf_string, hf->value);
		strcat(hf_string, "\r\n");
		hf = hf->next;
	}

	char *str = malloc(size);
	assert(str != NULL);
	memset(hf_string, 0, size);

	snprintf(str, LONG_STR, "GET %s HTTP/1.0\r\n%s\r\n",
			request->url->path, hf_string);

	free(hf_string);
	return str;
}



void gen_basic_request_header(request_t *request) {
	char *buf = malloc(sizeof(char) * SHORT_STR);
	assert(buf != NULL);

	header_field_t *ptr;

	// add Host
	if (strcmp(request->url->port, "80") == 0) {
		request->hf = new_header_field("Host", request->url->host);
	} else {
		snprintf(buf, SHORT_STR, "%s:%s",
				request->url->host, request->url->port);
		request->hf = new_header_field("Host", buf);
	}
	// add Connection
	ptr = request->hf;
	ptr->next = new_header_field("Connection", "close");
	// add user-agent
	ptr = ptr->next;
	ptr->next = new_header_field("User-Agent", "ao/pre-alpha");

	free(buf);
}
