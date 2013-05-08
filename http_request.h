struct request_t {
	char data[LONG_STR];
	header_field_t *hf;
	url_t *url;
};

request_t *new_request(void);
void del_request(request_t *req);

void gen_request_header(request_t *req);
void set_range(request_t *req, off_t start, off_t stop);
void request2string(request_t *req);
