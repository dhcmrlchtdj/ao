#include "ao.h"


response_t *new_response(char *str) {
	response_t *new = malloc(sizeof(response_t));
	assert(new != NULL);
	new->hf = NULL;
	filter_response_header(new->string, str);
	parse_response_header(new);
	return new;
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


void filter_response_header(char *desc, char *src) {
	// FIXME
	// error with src?
	int stop_flag = 0;
	while (stop_flag != 4) {
		switch (*src) {
			case '\r':
			case '\n':
				stop_flag++;
				break;
			default:
				stop_flag = 0;
				break;
		}
		*desc++ = *src++;
	}
	*desc = '\0';
}
