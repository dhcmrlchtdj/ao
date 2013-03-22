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




char *get_filename(char *str) {
	char *p = strrchr(str, '/');
	if (p == NULL || *++p == '\0')
		return "default";

	// FIXME
	char *q = strchr(p, '?');
	return q ? copy_str(p, q - p) : copy_str(p, strlen(p));
}



int conn_url(tasklet_t *tasklet, char *url) {
	int status;
	short redirection = 5;
	
	while (redirection) {
		parse_url(tasklet, url);

		create_tcp_conn(tasklet);

		gen_basic_request_header(tasklet); // such as Host, Connection ...
		send_request(tasklet);

		filter_response_string(tasklet);
		parse_response_string(tasklet);

		status = tasklet->response->status[0];
		if (status == '2') {
			return 0;
		} else if (status == '3' && redirection > 1) {
			// FIXME: bug?
			url = get_header_field(tasklet->response->hf, "Location");
			if (url == NULL)
				return -1;

			redirection--;

			free_url_t(tasklet->url);
			free_request_t(tasklet->request);
			free_response_t(tasklet->response);
		} else {
			free_url_t(tasklet->url);
			free_request_t(tasklet->request);
			free_response_t(tasklet->response);
			break;
		}
	}
	fprintf(stderr, "conn't connect to url: %s\n", url);
	return -1;
}
