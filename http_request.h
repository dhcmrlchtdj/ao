struct request_t {
	char *request;
	header_field_t *hf;
};

request_t *init_request_t(void);

void free_request_t(request_t *request);

void gen_basic_request_header(tasklet_t *tasklet);

void send_request(tasklet_t *tasklet);
