#include "ao.h"


void conn_url(Task *task, char *str) {
	int status;
	short redirection = 0;
	char *url = dynamic_copy(str, strlen(str));
	
	while (1) {
		parse_url(task, url);
		free(url);

		create_tcp_conn(task);

		gen_basic_request_header(task); // such as Host, Connection ...
		send_request(task);

		filter_response_header(task);
		parse_response_header(task);

		status = task->response->status[0];
		if (status == '2') {
			break;
		} else if (status != '3') {
			fprintf(stderr, "conn url error. status is %c\n", status);
			printf("%s%s", task->request->string, task->response->string);
			exit(EXIT_FAILURE);
		} else if (redirection > MAX_REDIRECTION) {
			fprintf(stderr, "[ao] redirection too many times\n");
			exit(EXIT_FAILURE);
		} else {
			str = get_header_field(task->response->hf, "Location");
			if (str == NULL) {
				fprintf(stderr, "[ao] redirection error. "
						"not found Location\n");
				exit(EXIT_FAILURE);
			} else {
				redirection++;
				url = dynamic_copy(str, strlen(str));
				clear_Task(task);
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
		static_copy(ao->filename, SHORT_STR, "default", 7);
	} else {
		// FIXME
		stop = strchr(start, '?');
		if (stop) {
			// stop should back one step
			static_copy(ao->filename, SHORT_STR, start, (stop - 1) - start);
		} else {
			static_copy(ao->filename, SHORT_STR, start, strlen(start));
		}
	}
}



void get_filesize_by_range(AO *ao, char *range) {
	char *pos;
	pos = strchr(range, '/');
	pos++; // move ti filesize
	ao->filesize = strtoul(pos, NULL, 0);
}
