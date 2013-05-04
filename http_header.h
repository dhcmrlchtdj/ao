struct header_field_t {
	char *name;
	char *value;
	struct header_field_t *next;
};

header_field_t *new_header_field(char *name, char *value);
void del_header_field(header_field_t *hf);

char *get_header(header_field_t *hf, char *name);
void set_header(header_field_t *hf, char *name, char *value);

void _print_header_field(header_field_t *hf); // used to debug
