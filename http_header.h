struct HeaderField {
	char *name;
	char *value;
	struct HeaderField *next;
};

HeaderField *init_HeaderField(char *name, char *value);
void free_HeaderField(HeaderField *hf);

char *get_header_field(HeaderField *hf, char *name);

void _print_header_field(HeaderField *hf); // used to debug
