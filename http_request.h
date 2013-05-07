struct request_t {
	char request[LONG_STR];
	header_field_t *hf;
	url_t *url; // not free by del_request
};

request_t *new_request(void);
void del_request(request_t *req);
void gen_request_header(request_t *req);
void gen_request_string(request_t *req);

void set_range(request_t *req, off_t start, off_t stop);
