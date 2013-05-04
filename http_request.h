struct request_t {
	header_field_t *hf;
	url_t *url;
};

request_t *new_request(url_t *url);
void del_request(request_t *request);
void _gen_basic_header(request_t *request);

void set_range(request_t *request, off_t start, off_t stop);

void request2str(request_t *request, char *str);
