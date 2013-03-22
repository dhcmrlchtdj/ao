#define MAX_REDIRECTION 5

struct url_t {
	char host[50];
	char port[10];
	char path[1024];
};

url_t *init_url_t(void);
void parse_url(tasklet_t *tasklet, char *str);
void free_url_t(url_t *url);

void conn_url(tasklet_t *tasklet, char *url);

void get_filename_by_path(ao_t *ao);
void get_filesize_by_range(ao_t *ao, char *str);
