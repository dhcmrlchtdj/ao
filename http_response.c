#include "ao.h"


response_t *new_response(void) {
	response_t *resp = Malloc(sizeof(response_t));
	resp->hf = NULL;
	resp->data = new_response_data();
	return resp;
}



void del_response(response_t *response) {
	del_header_field(response->hf);
	free(response);
}



void string2response(response_t *resp) {
	// status code
	// strlen("http/1.x ") == 9
	// strlen("200") == 3
	static_copy(resp->status, HTTP_STATUS_LEN, resp->data + 9, 3);

	// header field
	header_field_t **ptr = &resp->hf; // pointer to address of hf
	// hf_string will be modified
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



char *new_response_data(void) {
	char *data = Malloc(RECV_SIZE * sizeof(char));
	return data;
}
