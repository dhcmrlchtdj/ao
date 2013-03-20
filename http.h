struct header_field_t {
	char *name;
	char *value;
	struct header_field_t *next;
};

header_field_t *_gen_header_field(char *name, char *value);
void set_header_field(header_field_t *hf, char *name, char *value);
char *get_header_field(header_field_t *hf, char *name);

void print_header_field(header_field_t *hf);


/* * * * * * * * * */


struct request_t {
	char *request;
	struct header_field_t *hf;
};

void free_request_t(request_t *request);

void init_request(ao_t *ao);
void send_request(ao_t *ao);


/* * * * * * * * * */


struct response_t {
	char status[4];
	struct header_field_t *hf;
};

void free_response_t(response_t *response);

void get_response_header(ao_t *ao);
void parse_response_header(ao_t *ao, char *s);
