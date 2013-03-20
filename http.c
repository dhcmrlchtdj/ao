#include "ao.h"


header_field_t *_gen_header_field(char *name, char *value) {
	header_field_t *hf = malloc(sizeof(header_field_t));
	hf->name = name;
	hf->value = value;
	hf->next = NULL;
	return hf;
}



void set_header_field(header_field_t *hf, char *name, char *value) {
	header_field_t *ptr;
	for (ptr = hf; ptr != NULL; ptr = ptr->next) {
		if (strcmp(ptr->name, name) == 0) {
			ptr->value = value;
			break;
		} else if (ptr->next == NULL) {
			ptr->next = _gen_header_field(name, value);
			break;
		}
	}
}



char *get_header_field(header_field_t *hf, char *name) {
	header_field_t *ptr;
	for (ptr = hf; ptr != NULL; ptr = ptr->next) {
		if (strcmp(ptr->name, name) == 0)
			return ptr->value;
	}
	return NULL;
}



void print_header_field(header_field_t *hf) {
	while (hf) {
		printf("%s: %s\n", hf->name, hf->value);
		hf = hf->next;
	}
}


/* * * * * * * * * */


void _gen_request(ao_t *ao) {
	ao->request->request = malloc(sizeof(char) * 1024);
	char *ptr = ao->request->request;
	*ptr = '\0';
	strcat(ptr, "GET ");
	strcat(ptr, ao->url->path);
	strcat(ptr, " HTTP/1.0\r\n");
	strcat(ptr, "Host: ");
	strcat(ptr, ao->url->host);
	strcat(ptr, ":");
	strcat(ptr, ao->url->port);
	strcat(ptr, "\r\n");

	header_field_t *tmp = ao->request->hf;
	while (tmp) {
		strcat(ptr, tmp->name);
		strcat(ptr, ": ");
		strcat(ptr, tmp->value);
		strcat(ptr, "\r\n");
		tmp = tmp->next;
	}
	strcat(ptr, "\r\n");
}



void init_request(ao_t *ao) {
	ao->request = malloc(sizeof(request_t));

	header_field_t *ptr = _gen_header_field("Accept",
			"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	ao->request->hf = ptr; // assign to ao->request->hf

	ptr->next = _gen_header_field("User-Agent",
			"Opera/9.80 (X11; Linux x86_64) Presto/2.12.388 Version/12.14");

	ptr = ptr->next;
	ptr->next = _gen_header_field("Connection", "close");

	ptr = ptr->next;
	ptr->next = _gen_header_field("Accept-Encoding", "identity");

	ptr = ptr->next;
	ptr->next = _gen_header_field("Accept-Language", "en");

	ptr = ptr->next;
	ptr->next = _gen_header_field("DNT", "1");
}



void send_request(ao_t *ao) {
	_gen_request(ao);
	send_all(ao->sockfd, ao->request->request, strlen(ao->request->request));
}



void free_request_t(request_t *request) {
	free(request->request);
	header_field_t *ptr = request->hf;
	while (ptr) {
		request->hf = ptr->next;
		free(ptr);
		ptr = request->hf;
	}
	free(request);
}


/* * * * * * * * * */


void get_response_header(ao_t *ao) {
	char buf[4096];
	int flag_stop = 0;
	int pos = 0;
	while (flag_stop != 4) {
		_recv(ao->sockfd, buf + pos, 1);
		switch (buf[pos]) {
			case '\r':
			case '\n':
				flag_stop++;
				break;
			default:
				flag_stop = 0;
		}
		pos++;
	}
	buf[pos] = '\0';

	parse_response_header(ao, buf);
}



void parse_response_header(ao_t *ao, char *header_string) {
	ao->response = malloc(sizeof(response_t));
	// 返回值 status code
	memcpy(ao->response->status, header_string + 9, 3);
	ao->response->status[3] = '\0';

	header_field_t **ptr = &(ao->response->hf);

	char *p, *q;
	p = strchr(header_string, '\n');
	while (1) {
		q = strchr(++p, ':');
		if (q == NULL) break;
		// name: value == p: q+2
		*ptr = _gen_header_field(p, q + 2);
		// 获取 next 指针的地址
		ptr = &(*ptr)->next;

		*q++ = '\0';
		p = strchr(q, '\r');
		*p++ = '\0';
	}
}





void free_response_t(response_t *response) {
	header_field_t *ptr = response->hf;
	while (ptr) {
		response->hf = ptr->next;
		free(ptr);
		ptr = response->hf;
	}
	free(response);
}
