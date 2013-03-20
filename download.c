#include "ao.h"


url_t *parse_url(char *url) {
	char host[50];
	char port[10] = "80";
	char path[1024] = "/";

	char *with_port = "http://%[^:/]:%[^/]%[^ ]";
	char *no_port = "http://%[^/]%[^ ]";

	int cnt = sscanf(url, with_port, host, port, path);
	if (cnt != 3) { // url without port
		cnt = sscanf(url, no_port, host, path);
		if (cnt < 1) return NULL;
	}

	url_t *u = malloc(sizeof(url_t));
	u->host = _copy_str(host, strlen(host));
	u->port = _copy_str(port, strlen(port));
	u->path = _copy_str(path, strlen(path));

	return u;
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


/* * * * * * * * * */


void download_by_url(char *url) {
	ao_t *ao = malloc(sizeof(ao_t));

	// 分析链接
	ao->url = parse_url(url);
	if (ao->url == NULL) {
		fprintf(stderr, "Invalid url: %s\n", url);
		free(ao);
		exit(EXIT_FAILURE);
	}
	// 获取文件名
	ao->filename = get_filename(ao->url->path);

	// 建立 tcp 连接
	ao->sockfd = tcp_conn(ao->url);
	if (ao->sockfd == -1) {
		fprintf(stderr, "conn_url error\n");
		free_url_t(ao->url);
		free(ao);
		exit(EXIT_FAILURE);
	}

	// 发送 http 请求
	init_request(ao);
	send_request(ao);

	// 处理回应
	get_response_header(ao);

	if (ao->response->status[0] == '2') {
		// 打开文件进行读写
		ao->file = fopen(ao->filename, "a+b");
		if (ao->file == NULL) {
			fclose(ao->file);
			free_ao_t(ao);
			perror("fopen error: ");
			exit(EXIT_FAILURE);
		} else {
			save(ao);
			fclose(ao->file);
			free_ao_t(ao);
		}
	} else {
		printf("cannot down\n");
		free_ao_t(ao);
	}
}


/* * * * * * * * * */


void save(ao_t *ao) {
	char buf[4096];
	ssize_t s;
	while (1) {
		s = _recv(ao->sockfd, buf, 4096);
		if (s == 0)
			break;

		fwrite(buf, sizeof(char), s, ao->file);
		printf("download speed: %ld\n", s);
	}
}

