struct response_t {
	char status[4];
	char *string; // caution
	header_field_t *hf;
};

void free_response_t(response_t *response);

void filter_response_string(tasklet_t *tasklet);
void parse_response_string(tasklet_t *tasklet);
