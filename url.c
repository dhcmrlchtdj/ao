#include "ao.h"


url_t *init_url_t(void) {
	url_t *url = malloc(sizeof(url_t));
	return url;
}



void parse_url(ao_t *ao, char *str) {
	char host[50];
	char port[10] = "80";
	char path[1024] = "/";

	char *with_port = "http://%[^:/]:%[^/]%[^ ]";
	char *no_port = "http://%[^/]%[^ ]";

	int cnt = sscanf(str, with_port, host, port, path);
	if (cnt != 3) { // url without port
		cnt = sscanf(str, no_port, host, path);
		if (cnt < 1) {
			fprintf(stderr, "Invalid url: %s\n", str);
			exit(EXIT_FAILURE);
		}
	}

	ao->url->host = _copy_str(host, strlen(host));
	ao->url->port = _copy_str(port, strlen(port));
	ao->url->path = _copy_str(path, strlen(path));
}



void free_url_t(url_t *url) {
	free(url->host);
	free(url->port);
	free(url->path);
	free(url);
}











char *get_filename(char *str) {
	char *p = strrchr(str, '/');
	if (p == NULL || *++p == '\0')
		return "default";

	// FIXME
	char *q = strchr(p, '?');
	return q ? _copy_str(p, q - p) : _copy_str(p, strlen(p));
}
