#include "ao.h"


response_t *new_response(void) {
	response_t *resp = malloc(sizeof(response_t));
	assert(resp != NULL);
	resp->hf = NULL;
	return resp;
}



void del_response(response_t *response) {
	del_header_field(response->hf);
	free(response);
}



void parse_response_header(response_t *resp) {
	// status code
	static_copy(resp->status, 4, resp->data + 9, 3);

	// header field
	header_field_t **ptr = &resp->hf; // pointer to address of hf
	char *hf_string = dynamic_copy(resp->data, strlen(resp->data));

	char *name, *value, *stop;
	stop = strchr(hf_string, '\n');
	name = stop + 1; // move to first header
	while (1) {
		stop = strchr(name, ':');
		if (stop == NULL) break; // no more header field
		*stop = '\0';
		value = stop + 2; // move to value
		stop = strchr(value, '\r');
		*stop = '\0';

		*ptr = new_header_field(name, value);
		ptr = &(*ptr)->next;

		name = stop + 2; // move to next header name
	}

	free(hf_string);
}
