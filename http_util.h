#define MAX_REDIRECTION 5

struct Url {
	char host[50];
	char port[10];
	char path[1024];
};

Url *init_url(void);
void parse_url(Task *task, char *str);
void free_url(Url *url);

void conn_url(Task *task, char *url);

void get_filename_by_path(AO *ao, char *path);
void get_filesize_by_range(AO *ao, char *range);
