#include "ao.h"


header_field_t *gen_header_field(char *name, char *value) {
	header_field_t *hf = malloc(sizeof(header_field_t));
	hf->name = _copy_str(name, strlen(name));
	hf->value = _copy_str(value, strlen(value));
	hf->next = NULL;
	return hf;
}



void free_header_field_t(header_field_t *hf) {
	header_field_t *ptr = hf;
	while (ptr) {
		hf = ptr->next;
		free(ptr->name);
		free(ptr->value);
		free(ptr);
		ptr = hf;
	}
}



char *get_header_field(header_field_t *hf, char *name) {
	while (hf) {
		if (strcasecmp(hf->name, name) == 0)
			return hf->value;
		else
			hf = hf->next;
	}
	return NULL;
}



void add_header_field(header_field_t *hf, char *name, char *value) {
	if (hf == NULL) {
		hf = gen_header_field(name, value);
	} else {
		while (hf->next)
			hf = hf->next;
		hf->next = gen_header_field(name, value);
	}
}



void set_header_field(header_field_t *hf, char *name, char *value) {
	while (hf) {
		if (strcasecmp(hf->name, name) == 0) {
			free(hf->value);
			hf->value = _copy_str(value, strlen(value));
			break;
		} else if (hf->next == NULL) {
			hf->next = gen_header_field(name, value);
			break;
		}
	}
}



char *header_to_string(header_field_t *hf) {
	char *hf_string = malloc(sizeof(char) * 4096);
	while (hf) {
		strcat(hf_string, hf->name);
		strcat(hf_string, ": ");
		strcat(hf_string, hf->value);
		strcat(hf_string, "\r\n");
		hf = hf->next;
	}
	return hf_string;
}

header_field_t *string_to_header(char *str);



int get_length(ao_t *ao) {
	char *p, *q;
	p = get_header_field(ao->response->hf, "Content-Range");
	if (p && (q = strchr(p, '/'))) {
		ao->length = strtoul(++q, NULL, 0);
		return 0;
	} else {
		ao->length = 0;
		return 1;
	}
}



void set_range(header_field_t *hf, char *start, char *stop) {
	char *buf = malloc(sizeof(char) * 50);
	memset(buf, 0, sizeof(char) * 50);
	memcpy(buf, "bytes=", 7);
	if (start) {
		strcat(buf, start);
		strcat(buf, "-");
		if (stop)
			strcat(buf, stop);
		set_header_field(hf, "Range", buf);
	}
}



void _print_header_field(header_field_t *hf) {
	while (hf) {
		printf("%s: %s\n", hf->name, hf->value);
		hf = hf->next;
	}
}


