#include "ao.h"


url_t *init_url(void) {
	url_t *url = malloc(sizeof(url_t));
	return url;
}



void free_url(url_t *url) {
	free(url);
}



void parse_url(task_t *task, char *str) {
	char *with_port = "http://%[^:/]:%[^/]%[^ ]";
	char *no_port = "http://%[^/]%[^ ]";

	int cnt = sscanf(str, with_port, task->url->host,
			task->url->port, task->url->path);

	if (cnt != 3) { // url without port
		cnt = sscanf(str, no_port, task->url->host, task->url->path);
		if (cnt != 2) {
			fprintf(stderr, "Invalid url: %s\n", str);
			exit(EXIT_FAILURE);
		}
		static_copy(task->url->port, 10,  "80", 2);
	}
}
