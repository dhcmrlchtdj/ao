struct url_t {
	char *host;
	char *port;
	char *path;
};

url_t *init_url_t(void);
void parse_url(ao_t *ao, char *str);
void free_url_t(url_t *url);
