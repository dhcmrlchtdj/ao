struct request_t {
	header_field_t *hf;
	url_t *url;
};

request_t *new_request(url_t *url);
void del_request(request_t *request);

char *request2str(request_t *request);
void gen_basic_request_header(request_t *request);
