struct request_t {
	url_t *url; // not free by del_request
	header_field_t *hf;
	char data[LONG_STR];
};

request_t *new_request(void);
void del_request(request_t *req);
void gen_request_header(request_t *req);
void request2string(request_t *req);

void set_range(request_t *req, off_t start, off_t stop);
