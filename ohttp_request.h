struct request_t {
	char string[LONG_STR];
	header_field_t *hf;
};

request_t *init_request(void);
void free_request(request_t *request);

void gen_basic_request_header(task_t *task);
void send_request(task_t *task);
