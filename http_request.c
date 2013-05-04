#include "ao.h"

request_t *new_request(url_t *url) {
	request_t *new = malloc(sizeof(request_t));
	assert(new != NULL);
	new->url = url;
	_gen_basic_header(new);
	return new;
}



void del_request(request_t *request) {
	del_header_field(request->hf);
	free(request);
}



void _gen_basic_header(request_t *request) {
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



void set_range(request_t *request, off_t start, off_t stop) {
	char value[SHORT_STR];
	snprintf(value, SHORT_STR, "bytes=%ld-%ld", start, stop);
	set_header(request->hf, "Range", value);
}



// convert request_t to string
void request2str(request_t *request, char *str) {
	char *hf_string = calloc(LONG_STR, sizeof(char));
	assert(hf_string != NULL);

	header_field_t *hf = request->hf;
	while (hf) {
		strcat(hf_string, hf->name);
		strcat(hf_string, ": ");
		strcat(hf_string, hf->value);
		strcat(hf_string, "\r\n");
		hf = hf->next;
	}

	snprintf(str, LONG_STR, "GET %s HTTP/1.0\r\n%s\r\n",
			request->url->path, hf_string);

	free(hf_string);
}
