struct Request {
	//char *request;
	Header *hf;
};

Request *init_request(void);

void free_request(Request *request);

void gen_basic_request_header(Task *task);

void send_request(Task *task);
