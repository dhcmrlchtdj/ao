struct Response {
	char status[4];
	char string[LONG_STR]; // caution
	HeaderField *hf;
};

Response *init_Response(void);
void free_Response(Response *response);

void filter_response_string(Task *task);
void parse_response_string(Task *task);
