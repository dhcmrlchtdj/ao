struct response_t {
	char status[4];
	header_field_t *hf;
	char string[LONG_STR];
};

response_t *new_response(void);
void del_response(response_t *response);

void parse_response_header(response_t *response);


//void filter_response_header(task_t *task);
