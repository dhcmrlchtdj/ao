#include "ao.h"


HeaderField *init_HeaderField(char *name, char *value) {
	HeaderField *hf = malloc(sizeof(HeaderField));
	hf->name = dynamic_copy(name, strlen(name));
	hf->value = dynamic_copy(value, strlen(value));
	hf->next = NULL;
	return hf;
}



void free_HeaderField(HeaderField *hf) {
	HeaderField *ptr = hf;
	while (ptr) {
		hf = ptr->next;
		free(ptr->name);
		free(ptr->value);
		free(ptr);
		ptr = hf;
	}
}



char *get_header_field(HeaderField *hf, char *name) {
	while (hf) {
		if (strcasecmp(hf->name, name) == 0)
			return hf->value;
		else
			hf = hf->next;
	}
	return NULL;
}



void _print_header(HeaderField *hf) {
	while (hf) {
		printf("%s: %s\n", hf->name, hf->value);
		hf = hf->next;
	}
}
