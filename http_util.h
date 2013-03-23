#define MAX_REDIRECTION 5

struct Url {
	char host[50];
	char port[10];
	char path[SHORT_STR];
};

Url *init_Url(void);
void free_Url(Url *url);
void parse_url(Task *task, char *str);

void conn_url(Task *task, char *url);

void get_filename_by_path(AO *ao, char *path);
void get_filesize_by_range(AO *ao, char *range);
