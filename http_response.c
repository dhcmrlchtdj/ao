#include "ao.h"


response_t *init_response_t(void) {
	response_t *res = malloc(sizeof(response_t));
	res->string = NULL;
	res->hf = NULL;
	return res;
}



void free_response_t(response_t *response) {
	free_header_field_t(response->hf);
	free(response->string);
	free(response);
}



void filter_response_string(ao_t *ao) {
	char *buff = malloc(sizeof(char) * 4096);
	short stop_flag = 0;
	int pos = 0;
	while (stop_flag != 4) {
		_recv(ao->sockfd, buff + pos, 1);
		switch (buff[pos]) {
			case '\r':
			case '\n':
				stop_flag++;
			default:
				stop_flag = 0;
		}
		pos++;
	}
	buff[pos] = '\0';
	ao->response->string = _copy_str(buff, pos);
}



void parse_response_string(ao_t *ao) {
	memcpy(ao->response->status, ao->response->string + 9, 3);
	ao->response->status[3] = '\0';

	char *p, *q, *r;
	p = strchr(ao->response->string, '\n');
	p++;
	while (1) {
		q = strchr(p, ':');
		if (q == NULL) break;
		*q = '\0';
		r = strchr(++q, '\r');
		*r = '\0';

		// name: value == p: q+1
		add_header_field(ao->response->hf, p, ++q);

		p = r + 2;
	}
}
