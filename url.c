#include "ao.h"


Url *init_Url(void) {
	Url *url = malloc(sizeof(Url));
	return url;
}



void free_Url(Url *url) {
	free(url);
}



void parse_url(Task *task, char *str) {
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
