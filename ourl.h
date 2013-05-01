struct url_t {
	char host[50];
	char port[10];
	char path[SHORT_STR];
};

url_t *init_url(void);
void free_url(url_t *url);


void parse_url(task_t *task, char *str);
