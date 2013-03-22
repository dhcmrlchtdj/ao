struct url_t {
	char host[50];
	char port[10];
	char path[1024];
};

url_t *init_url_t(void);
void parse_url(tasklet_t *tasklet, char *str);
void free_url_t(url_t *url);
char *get_filename(char *str);

int conn_url(tasklet_t *tasklet, char *url);
