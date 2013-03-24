#include "ao.h"


header_field_t *init_header_field(char *name, char *value) {
	header_field_t *hf = malloc(sizeof(header_field_t));
	hf->name = dynamic_copy(name, strlen(name));
	hf->value = dynamic_copy(value, strlen(value));
	hf->next = NULL;
	return hf;
}



void free_header_field(header_field_t *hf) {
	header_field_t *ptr = hf;
	while (ptr) {
		hf = ptr->next;
		free(ptr->name);
		free(ptr->value);
		free(ptr);
		ptr = hf;
	}
}



char *get_header(header_field_t *hf, char *name) {
	while (hf) {
		if (strcasecmp(hf->name, name) == 0)
			return hf->value;
		else
			hf = hf->next;
	}
	return NULL;
}




void _print_header_field(header_field_t *hf) {
	while (hf) {
		printf("%s: %s\n", hf->name, hf->value);
		hf = hf->next;
	}
}
