struct Header {
	char *name;
	char *value;
	struct Header *next;
};

Header *init_header(char *name, char *value);
void free_header(Header *hf);


char *get_header(Header *hf, char *name);
void set_header(Header *hf, char *name, char *value);


char *header_to_string(Header *hf);
Header *string_to_header(char *str);


void _print_header_field(Header *hf); // used to debug
