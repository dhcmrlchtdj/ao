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



void filter_response_string(Task *task) {
	char *buff = malloc(sizeof(char) * LONG_STR);
	short stop_flag = 0;
	int pos = 0;
	while (stop_flag != 4) {
		_recv(task->sockfd, buff + pos, 1);
		switch (buff[pos]) {
			case '\r':
			case '\n':
				stop_flag++;
				break;
			default:
				stop_flag = 0;
				break;
		}
		pos++;
	}
	buff[pos] = '\0';
	static_copy(task->response->string, LONG_STR, buff, pos);
	free(buff);
}



void parse_response_string(Task *task) {
	static_copy(task->response->status, LONG_STR,
			task->response->string + 9, 3);

	char *name, *value, *stop;
	HeaderField **ptr = &task->response->hf;
	name = strchr(task->response->string, '\n');
	name += 2;
	while (1) {
		value = strchr(name, ':');
		if (value == NULL) break;
		*value = '\0';
		value += 2;
		stop = strchr(value, '\r');
		*stop = '\0';

		*ptr = init_HeaderField(name, value);
		ptr = &(*ptr)->next;

		name = stop + 2;
	}
}
