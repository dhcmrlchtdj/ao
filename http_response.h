#define HTTP_STATUS_LEN 4

struct response_t {
	header_field_t *hf;
	char status[HTTP_STATUS_LEN];
	char *data; // char data[RECV_SIZE]
};

response_t *new_response(void);
void del_response(response_t *resp);

void string2response(response_t *resp);

char *new_response_data(void);
