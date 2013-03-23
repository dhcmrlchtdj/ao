#include "ao.h"


Url *init_url(void) {
	Url *url = malloc(sizeof(Url));
	return url;
}



void parse_url(Task *task, char *str) {
	char *with_port = "http://%[^:/]:%[^/]%[^ ]";
	char *no_port = "http://%[^/]%[^ ]";

	int cnt = sscanf(str, with_port, task->url->host,
		   	task->url->port, task->url->path);
	if (cnt != 3) { // url without port
		cnt = sscanf(str, no_port, task->url->host, task->url->path);
		memcpy(task->url->port, "80", 3);
		if (cnt != 2) {
			fprintf(stderr, "Invalid url: %s\n", str);
			exit(EXIT_FAILURE);
		}
	}
}



void free_url(Url *url) {
	free(url);
}



void conn_url(Task *task, char *str) {
	int status;
	short redirection = 0;
	char *url = copy_str(str, strlen(str));
	
	while (1) {
		parse_url(task, url);
		free(url);

		create_tcp_conn(task);

		gen_basic_request_header(task); // such as Host, Connection ...
		send_request(task);

		filter_response_string(task);
		parse_response_string(task);

		status = task->response->status[0];
		if (status == '2') {
			break;
		} else if (status != '3') {
			fprintf(stderr, "conn url error. status is %c\n", status);
			exit(EXIT_FAILURE);
		} else if (redirection > MAX_REDIRECTION) {
			fprintf(stderr, "[ao] redirection too many times\n");
			exit(EXIT_FAILURE);
		} else {
			str = get_header(task->response->hf, "Location");
			if (str == NULL) {
				fprintf(stderr, "[ao] redirection error. "
						"not found Location\n");
				exit(EXIT_FAILURE);
			} else {
				redirection++;
				url = copy_str(str, strlen(str));
				clear_task(task);
				fprintf(stdout, "[ao] redirect to '%s', %d\n",
						url, redirection);
			}
		}
	}
}



void get_filename_by_path(AO *ao, char *path) {
	char *start, *stop;
	start = strrchr(path, '/');
	if (start == NULL || *++start == '\0') {
		// use "default" if not filename
		memcpy(ao->filename, "default", 8);
	} else {
		// FIXME ?
		stop = strchr(start, '?');
		if (stop) {
			memcpy(ao->filename, start, stop - start);
			ao->filename[stop - start] = '\0';
		} else {
			memcpy(ao->filename, start, strlen(start));
			ao->filename[strlen(start)] = '\0';
		}
	}
}



void get_filesize_by_range(AO *ao, char *range) {
	char *pos;
	pos = strchr(range, '/');
	pos++; // move ti filesize
	ao->filesize = strtoul(pos, NULL, 0);
}
