#include "ao.h"

int conn_url(ao_t *ao, char *str) {
	int status;
	short redirection = 5;
	
	char url[1024];
	memcpy(url, str, strlen(str) + 1);

	while (redirection) {
		memset(ao, 0, sizeof(ao_t));

		ao->url = init_url_t();
		parse_url(ao, url);

		create_tcp_conn(ao);

		ao->request = init_request_t();
		gen_basic_request_header(ao); // such as Host, Connection ...
		send_request(ao);

		ao->response = init_response_t();
		filter_response_string(ao);
		parse_response_string(ao);

		status = ao->response->status[0];
		if (status == '2') {
			return 0;
		} else if (status == '3') {
			str = get_header_field(ao->response->hf, "Location");
			if (str == NULL)
				return -1;

			memcpy(url, str, strlen(str) + 1);
			redirection--;

			free_url_t(ao->url);
			free_request_t(ao->request);
			free_response_t(ao->response);
		} else {
			free_url_t(ao->url);
			free_request_t(ao->request);
			free_response_t(ao->response);
			break;
		}
	}
	fprintf(stderr, "conn't connect to url: %s\n", url);
	return -1;
}
