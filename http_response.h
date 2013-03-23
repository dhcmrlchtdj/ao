struct Response {
	char status[4];
	char *string; // caution
	Header *hf;
};

Response *init_response(void);
void free_response(Response *response);

void filter_response_string(Task *task);
void parse_response_string(Task *task);
