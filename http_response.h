struct response_t {
	header_field_t *hf;
	char status[4];
	char *data;
};

response_t *new_response(void);
void del_response(response_t *resp);

void parse_response_header(response_t *resp);
