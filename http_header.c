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


/*
void add_header(Header *hf, char *name, char *value) {
	if (hf == NULL) {
		hf = init_header(name, value);
	} else {
		while (hf->next)
			hf = hf->next;
		hf->next = init_header(name, value);
	}
}



void set_header(Header *hf, char *name, char *value) {
	while (hf) {
		if (strcasecmp(hf->name, name) == 0) {
			free(hf->value);
			hf->value = copy_str(value, strlen(value));
			break;
		} else if (hf->next == NULL) {
			hf->next = init_header(name, value);
			break;
		}
	}
}
*/


char *header_to_string(HeaderField *hf) {
	char *hf_string = malloc(sizeof(char) * LONG_STR);
	memset(hf_string, 0, sizeof(char) * LONG_STR);
	while (hf) {
		strcat(hf_string, hf->name);
		strcat(hf_string, ": ");
		strcat(hf_string, hf->value);
		strcat(hf_string, "\r\n");
		hf = hf->next;
	}
	return hf_string;
}

/*Header *string_to_header(char *str);*/



/*void set_range(header_field_t *hf, char *start, char *stop) {*/
	/*char *buf = malloc(sizeof(char) * 50);*/
	/*memset(buf, 0, sizeof(char) * 50);*/
	/*memcpy(buf, "bytes=", 7);*/
	/*if (start) {*/
		/*strcat(buf, start);*/
		/*strcat(buf, "-");*/
		/*if (stop)*/
			/*strcat(buf, stop);*/
		/*set_header_field(hf, "Range", buf);*/
	/*}*/
/*}*/



void _print_header(HeaderField *hf) {
	while (hf) {
		printf("%s: %s\n", hf->name, hf->value);
		hf = hf->next;
	}
}


