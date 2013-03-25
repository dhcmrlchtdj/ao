#include "ao.h"


void conn_url(task_t *task) {
	int status;
	short redirection = 0;
	char *url = dynamic_copy(task->env->url, strlen(task->env->url));
	char *val;
	
	while (1) {
		parse_url(task, url);
		free(url);

		tcp_conn(task);

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
			val = get_header(task->response->hf, "Location");
			if (val == NULL) {
				fprintf(stderr, "[ao] redirection error. "
						"not found Location\n");
				exit(EXIT_FAILURE);
			} else {
				redirection++;
				url = dynamic_copy(val, strlen(val));
				clear_task(task);
				fprintf(stdout, "[ao] redirect to '%s', %d\n",
						url, redirection);
			}
		}
	}
}



void get_filename_by_path(env_t *env, char *path) {
	char *start, *stop;
	start = strrchr(path, '/');
	if (start == NULL || *++start == '\0') {
		// use "default" if not filename
		static_copy(env->filename, SHORT_STR, "default", 7);
	} else {
		// FIXME
		stop = strchr(start, '?');
		if (stop) {
			static_copy(env->filename, SHORT_STR, start, stop - start);
		} else {
			static_copy(env->filename, SHORT_STR, start, strlen(start));
		}
	}
}



void get_filesize_by_range(env_t *env, char *range) {
	char *pos;
	pos = strchr(range, '/');
	pos++; // move to filesize
	env->filesize = atol(pos);
}
