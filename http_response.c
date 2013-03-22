#include "ao.h"


response_t *init_response_t(void) {
	response_t *response = malloc(sizeof(response_t));
	response->hf = NULL;
	return response;
}



void free_response_t(response_t *response) {
	free_header_field_t(response->hf);
	free(response->string);
	free(response);
}



void filter_response_string(tasklet_t *tasklet) {
	char *buff = malloc(sizeof(char) * 4096);
	short stop_flag = 0;
	int pos = 0;
	while (stop_flag != 4) {
		_recv(tasklet->sockfd, buff + pos, 1);
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
	tasklet->response->string = copy_str(buff, pos);
	free(buff);

	/* DEBUG */
	printf("===response start===\n");
	printf("%s\n", tasklet->response->string);
	printf("===response end===\n");
}



void parse_response_string(tasklet_t *tasklet) {
	memcpy(tasklet->response->status, tasklet->response->string + 9, 3);
	tasklet->response->status[3] = '\0';

	char *name, *value, *stop;
	header_field_t **ptr = &tasklet->response->hf;
	name = strchr(tasklet->response->string, '\n');
	name += 2;
	while (1) {
		value = strchr(name, ':');
		if (value == NULL) break;
		*value = '\0';
		value += 2;
		stop = strchr(value, '\r');
		*stop = '\0';

		*ptr = gen_header_field(name, value);
		ptr = &(*ptr)->next;

		name = stop + 2;
	}
}
