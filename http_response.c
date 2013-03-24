#include "ao.h"


Response *init_Response(void) {
	Response *response = malloc(sizeof(Response));
	response->hf = NULL;
	return response;
}



void free_Response(Response *response) {
	free_HeaderField(response->hf);
	free(response);
}



void filter_response_header(Task *task) {
	char c;
	short stop_flag = 0;
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



void parse_response_header(Task *task) {
	// status code
	static_copy(task->response->status, LONG_STR,
			task->response->string + 9, 3);

	// header field
	char *hf_string = dynamic_copy(task->response->string,
			strlen(task->response->string));
	char *name, *value, *stop;
	HeaderField **ptr = &task->response->hf; // pointer to address of hf
	stop = strchr(hf_string, '\n');
	name = stop + 2;
	while (1) {
		stop = strchr(name, ':');
		if (stop == NULL) break; // no more header field
		*stop = '\0';
		value = stop + 2; // move to value
		stop = strchr(value, '\r');
		*stop = '\0';

		*ptr = init_HeaderField(name, value);
		ptr = &(*ptr)->next;

		name = stop + 2; // move to next header name
	}
	free(hf_string);
}
