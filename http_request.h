struct request_t {
	char *request;
	header_field_t *hf;
};

request_t *init_request_t(void);
void free_request_t(request_t *request);

void gen_basic_request_header(ao_t *ao);

void send_request(ao_t *ao);
