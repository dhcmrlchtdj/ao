struct url_t {
	char *host;
	char *port;
	char *path;
};

void parse_url(tasklet_t *tasklet, char *str);
void free_url_t(url_t *url);
char *get_filename(char *str);

int conn_url(tasklet_t *tasklet, char *url);
