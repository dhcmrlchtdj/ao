#include "ao.h"


Response *init_response(void) {
	Response *response = malloc(sizeof(Response));
	response->hf = NULL;
	return response;
}



void free_response(Response *response) {
	free_header(response->hf);
	free(response->string);
	free(response);
}



void filter_response_string(Task *task) {
	char *buff = malloc(sizeof(char) * 4096);
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
	task->response->string = copy_str(buff, pos);
	free(buff);
}



void parse_response_string(Task *task) {
	memcpy(task->response->status, task->response->string + 9, 3);
	task->response->status[3] = '\0';

	char *name, *value, *stop;
	Header **ptr = &task->response->hf;
	name = strchr(task->response->string, '\n');
	name += 2;
	while (1) {
		value = strchr(name, ':');
		if (value == NULL) break;
		*value = '\0';
		value += 2;
		stop = strchr(value, '\r');
		*stop = '\0';

		*ptr = init_header(name, value);
		ptr = &(*ptr)->next;

		name = stop + 2;
	}
}
