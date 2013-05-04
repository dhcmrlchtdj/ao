#include "ao.h"


response_t *new_response(void) {
	response_t *response = malloc(sizeof(response_t));
	assert(response != NULL);
	response->hf = NULL;
	return response;
}



void del_response(response_t *response) {
	del_header_field(response->hf);
	free(response);
}



void parse_response_header(response_t *response) {
	// status code
	static_copy(response->status, 4, response->string + 9, 3);

	// header field
	header_field_t **ptr = &response->hf; // pointer to address of hf
	char *hf_string = dynamic_copy(response->string, strlen(response->string));

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

/*
void filter_response_header(task_t *task) {
	char c;
	int stop_flag = 0;
	int pos = 0;
	while (stop_flag != 4) {
		_recv(task->sockfd, &c, 1);
		switch (c) {
			case '\r':
			case '\n':
				stop_flag++;
				break;
			default:
				stop_flag = 0;
				break;
		}
		task->response->string[pos] = c;
		pos++;
	}
	task->response->string[pos] = '\0';
}

*/
