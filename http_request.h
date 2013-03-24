struct Request {
	char string[LONG_STR];
	HeaderField *hf;
};

Request *init_Request(void);
void free_Request(Request *request);

void gen_basic_request_header(Task *task);

void send_request(Task *task);
