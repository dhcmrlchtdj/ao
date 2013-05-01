struct response_t {
	char status[4];
	char string[LONG_STR];
	header_field_t *hf;
};

response_t *init_response(void);
void free_response(response_t *response);

void filter_response_header(task_t *task);
void parse_response_header(task_t *task);
