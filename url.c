#include "ao.h"

url_t *new_url(void) {
	url_t *url = Malloc(sizeof(url_t));
	return url;
}



void del_url(url_t *url) {
	free(url);
}



// if url is invalid, return NULL
void parse_url(url_t *url, char *url_string) {
	memset(url, 0, sizeof(url_t));

	int cnt; // ssacnf result
	char *with_port = "%[^:]://%[^:/]:%[^/]%[^ ]";
	char *without_port = "%[^:]://%[^/]%[^ ]";

	cnt = sscanf(url_string, with_port,
			url->scheme, url->host, url->port, url->path);
	if (cnt != 4) { // url without port
		cnt = sscanf(url_string, without_port,
				url->scheme, url->host, url->path);
		if (cnt != 3) {
			fprintf(stderr, "[ao] Invalid url: '%s'\n", url_string);
			exit(EXIT_FAILURE);
		}
		// TODO
		// make compare faster
		if (strcasecmp(url->scheme, "http") == 0) {
			static_copy(url->port, PORT_LEN,  "80", 2);
		} else if (strcasecmp(url->scheme, "https") == 0) {
			static_copy(url->port, PORT_LEN,  "443", 3);
		} else {
			fprintf(stderr, "[ao] Invalid url: '%s'\n", url_string);
			exit(EXIT_FAILURE);
		}
	}
}
