struct response_t {
	char status[4];
	char *string; // caution
	header_field_t *hf;
};

response_t *init_response_t(void);
void free_response_t(response_t *response);

void filter_response_string(ao_t *ao);
void parse_response_string(ao_t *ao);
