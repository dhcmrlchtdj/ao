struct Url {
	char host[50];
	char port[10];
	char path[SHORT_STR];
};

Url *init_Url(void);
void free_Url(Url *url);
void parse_url(Task *task, char *str);
