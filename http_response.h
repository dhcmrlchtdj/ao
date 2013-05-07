struct response_t {
	char status[4];
	char response[LONG_STR];
	header_field_t *hf;
};

response_t *new_response(char *str);
void del_response(response_t *resp);

void parse_response_header(response_t *resp);

void filter_response_header(char *desc, char *src);
