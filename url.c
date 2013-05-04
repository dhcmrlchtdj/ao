#include "ao.h"

url_t *new_url(void) {
	url_t *new = malloc(sizeof(url_t));
	assert(new != NULL);
	return new;
}



void del_url(url_t *url) {
	free(url);
}


// if url is invalid, return NULL
url_t *parse_url(char *url) {
	url_t *u = new_url();

	int cnt; // ssacnf result
	char *with_port = "%[^:]://%[^:/]:%[^/]%[^ ]";
	char *without_port = "%[^:]://%[^/]%[^ ]";

	cnt = sscanf(url, with_port,
			u->scheme, u->host, u->port, u->path);
	if (cnt != 4) { // url without port
		cnt = sscanf(url, without_port,
				u->scheme, u->host, u->path);
		if (cnt != 3) {
			del_url(u);
			return NULL;
		}
		// TODO
		// make compare faster
		if (strcasecmp(u->scheme, "http") == 0) {
			static_copy(u->port, PORT_LEN,  "80", 2);
		} else if (strcasecmp(u->scheme, "https") == 0) {
			static_copy(u->port, PORT_LEN,  "443", 3);
		} else {
			del_url(u);
			return NULL;
		}
	}

	return u;
}
