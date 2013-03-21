struct header_field_t {
	char *name;
	char *value;
	struct header_field_t *next;
};

header_field_t *gen_header_field(char *name, char *value);
void free_header_field_t(header_field_t *hf);


char *get_header_field(header_field_t *hf, char *name);
void add_header_field(header_field_t *hf, char *name, char *value);
void set_header_field(header_field_t *hf, char *name, char *value);


char *header_to_string(header_field_t *hf);
header_field_t *string_to_header(char *str);


void _print_header_field(header_field_t *hf); // used to debug


void set_range(header_field_t *hf, char *start, char *stop);
int get_length(ao_t *ao);
