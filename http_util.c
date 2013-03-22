#include "ao.h"


url_t *init_url_t(void) {
	url_t *url = malloc(sizeof(url_t));
	return url;
}



void parse_url(tasklet_t *tasklet, char *str) {
	char *with_port = "http://%[^:/]:%[^/]%[^ ]";
	char *no_port = "http://%[^/]%[^ ]";

	int cnt = sscanf(str, with_port, tasklet->url->host,
			tasklet->url->port, tasklet->url->path);
	if (cnt != 3) { // url without port
		cnt = sscanf(str, no_port, tasklet->url->host, tasklet->url->path);
		memcpy(tasklet->url->port, "80", 3);
		if (cnt != 2) {
			fprintf(stderr, "Invalid url: %s\n", str);
			exit(EXIT_FAILURE);
		}
	}
}



void free_url_t(url_t *url) {
	free(url);
}



void conn_url(tasklet_t *tasklet, char *str) {
	int status;
	short redirection = 0;
	char *url = copy_str(str, strlen(str));
	
	while (1) {
		parse_url(tasklet, url);
		free(url);

		create_tcp_conn(tasklet);

		gen_basic_request_header(tasklet); // such as Host, Connection ...
		send_request(tasklet);

		filter_response_string(tasklet);
		parse_response_string(tasklet);

		status = tasklet->response->status[0];
		if (status == '2') {
			break;
		} else if (status != '3') {
			fprintf(stderr, "conn url error. status is %c\n", status);
			exit(EXIT_FAILURE);
		} else if (redirection > MAX_REDIRECTION) {
			fprintf(stderr, "[ao] redirection too many times\n");
			exit(EXIT_FAILURE);
		} else {
			str = get_header_field(tasklet->response->hf, "Location");
			if (str == NULL) {
				fprintf(stderr, "[ao] redirection error. "
						"not found Location\n");
				exit(EXIT_FAILURE);
			} else {
				redirection++;
				url = copy_str(str, strlen(str));
				clear_tasklet_t(tasklet);
				fprintf(stdout, "[ao] redirect to '%s', %d\n",
						url, redirection);
			}
		}
	}
}



void get_filename_by_path(ao_t *ao) {
	char *start = strrchr(ao->tasklets[0]->url->path, '/');
	if (start == NULL || *++start == '\0') {
		memcpy(ao->filename, "default", 8);
	} else {
		// FIXME ?
		char *stop = strchr(start, '?');
		if (stop) {
			memcpy(ao->filename, start, stop - start);
			ao->filename[stop - start] = '\0';
		} else {
			memcpy(ao->filename, start, strlen(start));
			ao->filename[strlen(start)] = '\0';
		}
	}
}



void get_filesize_by_range(ao_t *ao, char *str) {
	char *pos = strchr(str, '/');
	ao->filesize = strtoul(++pos, NULL, 0);
}
